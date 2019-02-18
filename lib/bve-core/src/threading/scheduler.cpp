#include <algorithm>
#include <core/threading/scheduler.hpp>

using bve::core::threading::TaskScheduler;

TaskScheduler::TaskScheduler(std::size_t const threads, std::size_t const blocking_threads) {
	restart(threads, blocking_threads);
}

void TaskScheduler::restart(std::size_t threads, std::size_t const blocking_threads) {
	assert(status_.load(std::memory_order_acquire) == Status::stopped);

	if (threads == 0) {
		threads = core_count;
	}

	status_.store(Status::paused, std::memory_order_release);

	threads_.reserve(threads - 1);
	blocking_threads_.reserve(blocking_threads);

	for (std::size_t i = 0; i < threads - 1; ++i) {
		threads_.emplace_back(std::thread([this] { this->taskExecutor(false); }));
	}
	for (std::size_t i = 0; i < blocking_threads; ++i) {
		blocking_threads_.emplace_back(std::thread([this] { this->blockingTaskExecutor(false); }));
	}

	// All inferior threads will wait in the OS.
}

void TaskScheduler::run() {
	status_.store(Status::running, std::memory_order_release);
	// Wake up sleepy threads
	{
		std::lock_guard l(executor_mutex_);
		executor_cv_.notify_all();
	}

	// Start processing
	Status const status = taskExecutor(true);

	if (status == Status::stopped) {
		// Join and delete all threads
		for (auto& thread : threads_) {
			thread.join();
		}
		for (auto& thread : blocking_threads_) {
			thread.join();
		}
		threads_.clear();
		blocking_threads_.clear();

		// We only get back here when the thread pool needs to die.
		// Pump the queue until it's empty
		Task* task;
		while (task_queue_.try_dequeue(task)) {
			delete task;
		}
		while (blocking_task_queue_.try_dequeue(task)) {
			delete task;
		}
	}
}

void TaskScheduler::pause() {
	status_.store(Status::paused, std::memory_order_release);
}

void TaskScheduler::stop() {
	status_.store(Status::stopped, std::memory_order_release);
}

template <bool Blocks>
FORCE_INLINE TaskScheduler::Status TaskScheduler::taskExecutorImpl(bool const main_thread) {
	// MSVC needs this. Don't ask.
	// ReSharper disable once CppRedundantQualifier
	bve::core::threading::Task* task = nullptr;
	Status status;
	while (true) {
		if constexpr (Blocks) {
			// We need to actually wait if we're blocking, we don't "own" a core.
			std::unique_lock l(executor_mutex_);
			while ((status = status_.load(std::memory_order_acquire)) == Status::running
			       && blocking_task_queue_.try_dequeue(task) == false) {
				executor_cv_.wait(l);
			}
		}
		else {
			while ((status = status_.load(std::memory_order_acquire)) == Status::running && task_queue_.try_dequeue(task) == false) {
				std::this_thread::yield();
			}
		}

		if (status == Status::paused) {
			// Sub threads need to wait, but main thread needs to return from run
			if (main_thread == false) {
				std::unique_lock l(executor_mutex_);
				while ((status = status_.load(std::memory_order_acquire)) == Status::paused) {
					executor_cv_.wait(l);
				}
				continue;
			}
			break;
		}
		if (status == Status::stopped) {
			break;
		}

		task->task_(*this);

		if (task->depender_ != nullptr) {
			std::size_t const ret = task->depender_->counter_.fetch_sub(1, std::memory_order_acq_rel);
			// If this is one we're the last one, and we need to add the real task to the queue
			if (ret == 1) {
				if (task->depender_->blocking_) {
					blocking_task_queue_.enqueue(task->depender_->actual_task_);
					std::lock_guard l(executor_mutex_);
					executor_cv_.notify_one();
				}
				else {
					task_queue_.enqueue(task->depender_->actual_task_);
				}

				std::lock_guard l(dependent_tasks_lock_);

				auto const itr = std::find_if(dependent_tasks_.begin(), dependent_tasks_.end(),
				                              [&](auto& u_ptr) { return u_ptr.get() == task->depender_; });
				dependent_tasks_.erase(itr);
			}
		}

		delete task;
		task = nullptr;
	}

	delete task;

	return status;
}

TaskScheduler::Status TaskScheduler::taskExecutor(bool const main_thread) {
	return taskExecutorImpl<false>(main_thread);
}

TaskScheduler::Status TaskScheduler::blockingTaskExecutor(bool const main_thread) {
	return taskExecutorImpl<true>(main_thread);
}
