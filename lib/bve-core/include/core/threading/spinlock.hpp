#pragma once

#include <atomic>
#include <emmintrin.h>
#include <mutex>
#include <thread>

namespace bve::core::threading {
	/**
	 * Count of cores on the current system. Used to prevent redundant calls.
	 */
	inline std::size_t core_count = std::thread::hardware_concurrency();

	/**
	 * \brief Simple spinlock. Falls back to a mutex if on a single core machine. Fulfills c++ named requirement: Mutex
	 */
	class Spinlock {
	  public:
		Spinlock() {
			if (core_count == 1) {
				mutex_ = std::make_unique<std::mutex>();
			}
		}

		Spinlock(Spinlock const&) = delete;
		Spinlock(Spinlock&&) = delete;
		Spinlock& operator=(Spinlock const&) = delete;
		Spinlock& operator=(Spinlock&&) = delete;

		~Spinlock() = default;

		/**
		 * Locks mutex. Blocks if mutex is unavailable.
		 */
		void lock() {
			if (mutex_ != nullptr) {
				mutex_->lock();
			}
			else {
				while (flag_.test_and_set(std::memory_order_acq_rel) == true) {
					_mm_pause();
				}
			}
		}

		// ReSharper disable once CppInconsistentNaming
		/**
		 * Attempt to lock the mutex
		 *
		 * @return If locking succeeded
		 */
		bool try_lock() {
			if (mutex_ != nullptr) {
				return mutex_->try_lock();
			}
			return flag_.test_and_set(std::memory_order_acq_rel) == false;
		}

		/**
		 * Unlocks mutex.
		 */
		void unlock() {
			if (mutex_ != nullptr) {
				mutex_->unlock();
			}
			else {
				flag_.clear(std::memory_order_release);
			}
		}

	  private:
		std::atomic_flag flag_;
		std::unique_ptr<std::mutex> mutex_ = nullptr;
	};
} // namespace bve::core::threading
