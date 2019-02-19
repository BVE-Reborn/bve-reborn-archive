#pragma once

#include <atomic>
#include <core/vtune/ittnottify.hpp>
#include <emmintrin.h>
#include <mutex>
#include <thread>

namespace bve::core::threading {
	/**
	 * Count of cores on the current system. Used to prevent redundant calls to std::thread::hardware_concurrency().
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
			__itt_sync_prepare(this);
			if (mutex_ != nullptr) {
				mutex_->lock();
			}
			else {
				while (flag_.test_and_set(std::memory_order_acq_rel) == true) {
					_mm_pause();
				}
			}
			__itt_sync_acquired(this);
		}

		// ReSharper disable once CppInconsistentNaming
		/**
		 * Attempt to lock the mutex
		 *
		 * @return If locking succeeded
		 */
		bool try_lock() {
			__itt_sync_prepare(this);
			bool ret;
			if (mutex_ != nullptr) {
				ret = mutex_->try_lock();
			}
			else {
				ret = flag_.test_and_set(std::memory_order_acq_rel) == false;
			}
			if (ret) {
				__itt_sync_acquired(this);
			}
			else {
				__itt_sync_cancel(this);
			}
			return ret;
		}

		/**
		 * Unlocks mutex.
		 */
		void unlock() {
			__itt_sync_releasing(this);
			if (mutex_ != nullptr) {
				mutex_->unlock();
			}
			else {
				flag_.clear(std::memory_order_release);
			}
		}

	  private:
		std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
		std::unique_ptr<std::mutex> mutex_ = nullptr;
	};
} // namespace bve::core::threading
