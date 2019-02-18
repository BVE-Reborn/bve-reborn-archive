#pragma once

#include <atomic>
#include <concurrentqueue.h>
#include <core/threading/spinlock.hpp>
#include <future>
#include <util/inlining_util.hpp>
#include <vector>

namespace bve::core::threading {
	class TaskScheduler;
	class DependentTask;

	class Task {
		using PackagedTask = std::packaged_task<void(TaskScheduler&)>;
		using Counter = std::atomic<std::size_t>;

	  public:
		friend TaskScheduler;
		friend DependentTask;

		Task(Task const& other) = delete;
		Task(Task&& other) noexcept = default;
		Task& operator=(Task const& other) = delete;
		Task& operator=(Task&& other) noexcept = default;

		~Task() = default;

	  private:
		Task() {}
		explicit Task(PackagedTask&& t, DependentTask* const depender) : task_(std::move(t)), depender_(depender) {}

		PackagedTask task_;
		DependentTask* depender_ = nullptr;
	};

	class DependentTask {
	  public:
		friend TaskScheduler;

		DependentTask(DependentTask const& other) = delete;
		DependentTask(DependentTask&& other) noexcept = delete;
		DependentTask& operator=(DependentTask const& other) = delete;
		DependentTask& operator=(DependentTask&& other) noexcept = delete;

	  private:
		explicit DependentTask(Task* const t, bool const blocking) : actual_task_(t), blocking_(blocking) {}

		Task* actual_task_;
		Task::Counter counter_ = 0;
		bool blocking_;
	};

	using DependentTaskHandle = std::unique_ptr<DependentTask>;

	class TaskScheduler {
	  public:
		TaskScheduler() = default;

		TaskScheduler(TaskScheduler const& other) = delete;
		TaskScheduler(TaskScheduler&& other) noexcept = delete;
		TaskScheduler& operator=(TaskScheduler const& other) = delete;
		TaskScheduler& operator=(TaskScheduler&& other) noexcept = delete;

		~TaskScheduler() = default;

		template <bool Blocks = false, class F>
		void addTask(F&& func) {
			if (stop_.load(std::memory_order_acquire) == true) {
				return;
			}
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), nullptr);
			if constexpr (Blocks) {
				blocking_task_queue_.enqueue(task);
				std::lock_guard l(blocking_task_mutex_);
				blocking_task_cv_.notify_one();
			}
			else {
				task_queue_.enqueue(task);
			}
		}

		template <bool Blocks = false, class F>
		void addTask(F&& func, DependentTaskHandle const& handle) {
			if (stop_.load(std::memory_order_acquire) == true) {
				return;
			}
			if (handle != nullptr) {
				handle->counter_.fetch_add(1, std::memory_order_acq_rel);
			}
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), handle.get());
			if constexpr (Blocks) {
				blocking_task_queue_.enqueue(task);
				std::lock_guard l(blocking_task_mutex_);
				blocking_task_cv_.notify_one();
			}
			else {
				task_queue_.enqueue(task);
			}
		}

		template <class F>
		FORCE_INLINE void addBlockingTask(F&& func) {
			addTask<true>(std::forward<F>(func));
		}

		template <class F>
		FORCE_INLINE void addBlockingTask(F&& func, DependentTaskHandle const& handle) {
			addTask<true>(std::forward<F>(func), handle);
		}

		void addDependentTask(DependentTaskHandle&& handle) {
			if (stop_.load(std::memory_order_acquire) == true) {
				return;
			}
			std::lock_guard l(dependent_tasks_lock_);

			dependent_tasks_.emplace_back(std::move(handle));
		}

		template <bool Blocks = false, class F>
		DependentTaskHandle prepareDependentTask(F&& func) {
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), nullptr);
			return std::unique_ptr<DependentTask>(new DependentTask(task, Blocks));
		}

		template <bool Blocks = false, class F>
		DependentTaskHandle prepareDependentTask(F&& func, DependentTaskHandle const& handle) {
			if (handle != nullptr) {
				handle->counter_.fetch_add(1, std::memory_order_acq_rel);
			}
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), handle.get());
			return std::unique_ptr<DependentTask>(new DependentTask(task, Blocks));
		}

		template <class F>
		FORCE_INLINE void prepareDependentBlockingTask(F&& func) {
			prepareDependentTask<true>(std::forward<F>(func));
		}

		template <class F>
		FORCE_INLINE void prepareDependentBlockingTask(F&& func, DependentTaskHandle const& handle) {
			prepareDependentTask<true>(std::forward<F>(func), handle);
		}

		void run(std::size_t threads, std::size_t blocking_threads);

		void stop();

	  private:
		struct ConcurrentQueueTraits : moodycamel::ConcurrentQueueDefaultTraits {
			static void* malloc(size_t const size) {
				return std::malloc(size);
			}
			static void free(void* ptr) {
				return std::free(ptr);
			}
		};

		void taskExecutor();
		void blockingTaskExecutor();

		template <bool Blocks>
		FORCE_INLINE void taskExecutorImpl();

		std::atomic<bool> stop_ = false;
		std::vector<std::thread> threads_;
		std::vector<std::thread> blocking_threads_;
		moodycamel::ConcurrentQueue<Task*, ConcurrentQueueTraits> task_queue_;
		moodycamel::ConcurrentQueue<Task*, ConcurrentQueueTraits> blocking_task_queue_;
		std::mutex blocking_task_mutex_;
		std::condition_variable blocking_task_cv_;
		Spinlock dependent_tasks_lock_{};
		std::vector<std::unique_ptr<DependentTask>> dependent_tasks_;
	};
} // namespace bve::core::threading
