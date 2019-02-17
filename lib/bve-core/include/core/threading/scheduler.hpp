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
		explicit Task(PackagedTask&& t, DependentTask* const counter) : task_(std::move(t)), depender_(counter) {}

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
		Task::Counter counter_;
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
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), nullptr);
			if constexpr (Blocks) {
				blocking_task_queue_.enqueue(task);
			}
			else {
				task_queue_.enqueue(task);
			}
		}

		template <class F>
		FORCE_INLINE void addBlockingTask(F&& func) {
			addTask<true>(std::forward<F>(func));
		}

		template <bool Blocks = false, class F>
		void addTask(F&& func, DependentTaskHandle const& handle) {
			if (handle != nullptr) {
				handle->counter_.fetch_add(1, std::memory_order_acq_rel);
			}
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), handle.get());
			if constexpr (Blocks) {
				blocking_task_queue_.enqueue(task);
			}
			else {
				task_queue_.enqueue(task);
			}
		}

		template <class F>
		FORCE_INLINE void addBlockingTask(F&& func, DependentTaskHandle const& handle) {
			addTask<true>(std::forward<F>(func), handle);
		}

		void addTask(DependentTaskHandle&& handle) {
			std::lock_guard l(dependent_tasks_lock_);

			dependent_tasks_.emplace_back(std::move(handle));
		}

		template <bool Blocks = false, class F>
		DependentTaskHandle prepareTask(F&& func) {
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), nullptr);
			return std::make_unique<DependentTask>(task, Blocks);
		}

		template <class F>
		FORCE_INLINE void prepareBlockingTask(F&& func) {
			prepareTask<true>(std::forward<F>(func));
		}

		void run(std::size_t threads, std::size_t blocking_threads);

		void stop();

	  private:
		struct ConcurrentQueueTraits : public moodycamel::ConcurrentQueueDefaultTraits {
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
		Spinlock dependent_tasks_lock_{};
		std::vector<std::unique_ptr<DependentTask>> dependent_tasks_;
	};
} // namespace bve::core::threading
