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

	/**
	 * Internal task structure to the TaskScheduler. Move-only.
	 */
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

		/**
		 * The actual packaged task that will be run.
		 */
		PackagedTask task_;
		/***
		 * A pointer to the dependent task that is waiting for me to finish.
		 */
		DependentTask* depender_ = nullptr;
	};

	/**
	 * Internal class to the TaskScheduler. Unmoveable, uncopyable.
	 * Thin wrapper around a Task that manages the information that releases it.
	 */
	class DependentTask {
	  public:
		friend TaskScheduler;

		DependentTask(DependentTask const& other) = delete;
		DependentTask(DependentTask&& other) noexcept = delete;
		DependentTask& operator=(DependentTask const& other) = delete;
		DependentTask& operator=(DependentTask&& other) noexcept = delete;

	  private:
		explicit DependentTask(Task* const t, bool const blocking) : actual_task_(t), blocking_(blocking) {}

		/***
		 * The actual task to run once all dependents finish.
		 */
		Task* actual_task_;
		/***
		 * The counter that is incremented for every dependent added and decremented when dependents finish. Will run when 0.
		 */
		Task::Counter counter_ = 0;
		/***
		 * If the task should be run on the blocking or non-blocking executors.
		 */
		bool blocking_;
	};

	/**
	 * Handle referring to a task that can have dependencies. Treat as if opaque.
	 * Use it to give the task dependencies, or to add the task to run.
	 */
	using DependentTaskHandle = std::unique_ptr<DependentTask>;

	/**
	 * \brief Thread pool that includes blocking and non-blocking executors.
	 * The API is fairly intuitive and is recursive: i.e. you can add more tasks from within tasks.
	 *
	 * Started via the \ref run function, which blocks, and will only terminate when the \ref stop function is called. This means on the
	 * tasks on the pool must call the \ref stop function.
	 *
	 * Tasks may be added before or after running starts, however adding tasks after the pool has stopped will fail.
	 *
	 * Tasks may depend on other tasks. The method for this is slightly inverted. You first have to make the task that you want to have
	 * dependencies and get a handle to it. Only then do you make the dependencies using that handle. Once all these dependencies have been
	 * added, you add the handle to the pool, allowing the task to run. Example below.
	 *
	 * ```
	 * TaskScheduler ts;
	 * DependentTaskHandle final = ts.prepareDependentTask([](TaskScheduler& ts) {
	 *                                                         std::cout << "I'm the second task\n";
	 *                                                         ts.stop();
	 *                                                     }); // Prepare task that will wait for others
	 * ts.addTask([](auto&) { std::cout << "I'm the first task!\n"; }, final); // Make final wait for completion of this task.
	 * ts.addDependentTask(std::move(final)); // Add final task to thread pool. May not use handle after this.
	 * ts.run(2, 2); // Run pool
	 * ```
	 */
	class TaskScheduler {
	  public:
		TaskScheduler() = default;

		TaskScheduler(TaskScheduler const& other) = delete;
		TaskScheduler(TaskScheduler&& other) noexcept = delete;
		TaskScheduler& operator=(TaskScheduler const& other) = delete;
		TaskScheduler& operator=(TaskScheduler&& other) noexcept = delete;

		~TaskScheduler() = default;

		/**
		 * Add a regular task. This task will get run on the regular executors, so it should not block.
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam Blocks For internal use only. Makes the task block.
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 */
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

		/**
		 * Add a regular task that another task depends on. This task will get run on the regular executors, so it should not block.
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam Blocks For internal use only. Makes the task block.
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 * @param handle  Handle to the task that should wait on this task.
		 */
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

		/**
		 * Add a blocking task. This task will get run on the blocking executors, so it should spend the majority of the time blocked.
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 */
		template <class F>
		FORCE_INLINE void addBlockingTask(F&& func) {
			addTask<true>(std::forward<F>(func));
		}

		/**
		 * Add a blocking task that another task depends on. This task will get run on the blocking executors, so it should spend the
		 * majority of the time blocked.
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 * @param handle  Handle to the task that should wait on this task.
		 */
		template <class F>
		FORCE_INLINE void addBlockingTask(F&& func, DependentTaskHandle const& handle) {
			addTask<true>(std::forward<F>(func), handle);
		}

		/**
		 * Add a dependent task to the pool. It will execute when all of the tasks that it depends on have finished. It will run on the
		 * executors it was made for.
		 *
		 * The handle needs to be moved in as the handle will be unsafe to use after this function is called.
		 *
		 * This function is fully thread safe.
		 *
		 * @param handle Handle to the task that should be added to the pool.
		 */
		void addDependentTask(DependentTaskHandle&& handle) {
			if (stop_.load(std::memory_order_acquire) == true) {
				return;
			}
			std::lock_guard l(dependent_tasks_lock_);

			dependent_tasks_.emplace_back(std::move(handle));
		}

		/**
		 * Prepare a regular task that will wait on other tasks to finish. This task will get run on the regular executors, so it should not
		 * block.
		 * **THIS DOES NOT ADD THE TASK TO THE QUEUE**. This handle allows you to add tasks for it to wait on. Once you are done adding
		 * tasks for it to wait on, you should add it to the queue using \ref addDependentTask(DependentTaskHandle&&) "addDependentTask".
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time after it
		 * is added.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam Blocks For internal use only. Makes the task block.
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 * @return        The handle to the dependent function.
		 */
		template <bool Blocks = false, class F>
		DependentTaskHandle prepareDependentTask(F&& func) {
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), nullptr);
			return std::unique_ptr<DependentTask>(new DependentTask(task, Blocks));
		}

		/**
		 * Prepare a regular task that will wait on other tasks to finish and will be waited on by another task.
		 * This task will get run on the regular executors, so it should not block.
		 * **THIS DOES NOT ADD THE TASK TO THE QUEUE**. This handle allows you to add tasks for it to wait on. Once you are done adding
		 * tasks for it to wait on, you should add it to the queue using \ref addDependentTask(DependentTaskHandle&&) "addDependentTask".
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time after it
		 * is added.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam Blocks For internal use only. Makes the task block.
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 * @param handle  Handle to the task that should wait on this task.
		 * @return        The handle to the dependent function.
		 */
		template <bool Blocks = false, class F>
		DependentTaskHandle prepareDependentTask(F&& func, DependentTaskHandle const& handle) {
			if (handle != nullptr) {
				handle->counter_.fetch_add(1, std::memory_order_acq_rel);
			}
			Task::PackagedTask packaged(func);
			auto* task = new Task(std::move(packaged), handle.get());
			return std::unique_ptr<DependentTask>(new DependentTask(task, Blocks));
		}

		/**
		 * Prepare a blocking task that will wait on other tasks to finish.
		 * This task will get run on the blocking executors, so it should spend the majority of the time blocked.
		 * **THIS DOES NOT ADD THE TASK TO THE QUEUE**. This handle allows you to add tasks for it to wait on. Once you are done adding
		 * tasks for it to wait on, you should add it to the queue using \ref addDependentTask(DependentTaskHandle&&) "addDependentTask".
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time after it
		 * is added.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 * @return        The handle to the dependent function.
		 */
		template <class F>
		FORCE_INLINE void prepareDependentBlockingTask(F&& func) {
			prepareDependentTask<true>(std::forward<F>(func));
		}

		/**
		 * Prepare a blocking task that will wait on other tasks to finish and will be waited on by another task.
		 * This task will get run on the blocking executors, so it should spend the majority of the time blocked.
		 * **THIS DOES NOT ADD THE TASK TO THE QUEUE**. This handle allows you to add tasks for it to wait on. Once you are done adding
		 * tasks for it to wait on, you should add it to the queue using \ref addDependentTask(DependentTaskHandle&&) "addDependentTask".
		 *
		 * A task must be callable with the signature `void(TaskScheduler&)`.
		 *
		 * If adding a lambda, everything should be passed by value as the Task will not execute for an unspecified amount of time after it
		 * is added.
		 *
		 * This function is fully thread safe.
		 *
		 * @tparam F      Type of function to call.
		 * @param func    The function to call.
		 * @param handle  Handle to the task that should wait on this task.
		 * @return        The handle to the dependent function.
		 */
		template <class F>
		FORCE_INLINE void prepareDependentBlockingTask(F&& func, DependentTaskHandle const& handle) {
			prepareDependentTask<true>(std::forward<F>(func), handle);
		}

		/**
		 * Run the task scheduler. The current thread will turn into a worker thread until the scheduler finishes execution. The only way to
		 * stop the scheduler is through the \ref stop function.
		 *
		 * This function is **thread unsafe**, but **not thread hostile**. There should only be one run function executing at a time.
		 *
		 * @param threads          Amount of primary worker threads. Run non-blocking tasks. Generally equal to core count.
		 * @param blocking_threads Amount of blocking worker threads. Run blocking tasks. Generally much more numerous than \p threads.
		 */
		void run(std::size_t threads, std::size_t blocking_threads);

		/**
		 * Terminate the task scheduler. Any tasks currently in flight will continue. All tasks not yet executed or tasks not yet added will
		 * not be run.
		 *
		 * This is the only way to terminate the execution of \ref run.
		 *
		 * This function is fully thread safe.
		 */
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
