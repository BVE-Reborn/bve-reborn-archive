#include <algorithm>
#include <core/threading/scheduler.hpp>

void bve::core::threading::TaskScheduler::run(std::size_t const threads, std::size_t const blocking_threads) {
	threads_.reserve(threads);
	blocking_threads_.reserve(blocking_threads);

	for (std::size_t i = 0; i < threads - 1; ++i) {
		threads_.emplace_back(std::thread([this] { this->taskExecutor(); }));
	}
	for (std::size_t i = 0; i < threads; ++i) {
		blocking_threads_.emplace_back(std::thread([this] { this->blockingTaskExecutor(); }));
	}

	// Start processing
	taskExecutor();
}

void bve::core::threading::TaskScheduler::stop() {
	stop_.store(true, std::memory_order_release);
}

template <bool Blocks>
FORCE_INLINE void bve::core::threading::TaskScheduler::taskExecutorImpl() {
	Task* task;
	while (true) {
		bool has_task = false;
		if constexpr (Blocks) {
			while (stop_.load(std::memory_order_acquire) == false && (has_task = blocking_task_queue_.try_dequeue(task)) == false) {
				std::this_thread::yield();
			}
		}
		else {
			while (stop_.load(std::memory_order_acquire) == false && (has_task = task_queue_.try_dequeue(task)) == false) {
				std::this_thread::yield();
			}
		}

		// The only reason we'd be here is if we're done.
		if (!has_task) {
			break;
		}

		task->task_(*this);

		if (task->depender_ != nullptr) {
			std::size_t const ret = task->depender_->counter_.fetch_sub(1, std::memory_order_acq_rel);
			// If this is one we're the last one, and we need to add the real task to the queue
			if (ret == 1) {
				if (task->depender_->blocking_) {
					blocking_task_queue_.enqueue(task->depender_->actual_task_);
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
	}

	delete task;
}

void bve::core::threading::TaskScheduler::taskExecutor() {
	taskExecutorImpl<false>();
}

void bve::core::threading::TaskScheduler::blockingTaskExecutor() {
	taskExecutorImpl<true>();
}
