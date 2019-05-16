#pragma once

#include <EABase/config/eacompilertraits.h>
#include <EASTL/internal/config.h>
#include <cstdlib>

namespace bve::util {
	class Allocator {
	  public:
		Allocator() = default;
		Allocator(Allocator const&) = delete;
		Allocator(Allocator&&) = delete;
		Allocator& operator=(Allocator const&) = delete;
		Allocator& operator=(Allocator&&) = delete;

		virtual void* allocate(size_t n, int flags = 0) = 0;
		virtual void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0) = 0;
		virtual void deallocate(void* p, size_t n) = 0;
	};

	class AllocatorHandle {
	  public:
		EA_FORCE_INLINE EASTL_ALLOCATOR_EXPLICIT AllocatorHandle(char const* = nullptr) : allocator_(nullptr) {}

		EA_FORCE_INLINE AllocatorHandle(Allocator* allocator, char const* = nullptr) : allocator_(allocator) {}

		EA_FORCE_INLINE AllocatorHandle(const AllocatorHandle& x, const char* = nullptr) : allocator_(x.allocator_) {}

		EA_FORCE_INLINE ~AllocatorHandle() = default;

		EA_FORCE_INLINE AllocatorHandle& operator=(const AllocatorHandle& x) = default;

		EA_FORCE_INLINE void* allocate(size_t n, int flags = 0) {
			return allocator_->allocate(n, flags);
		}

		EA_FORCE_INLINE void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0) {
			return allocator_->allocate(n, alignment, offset, flags);
		}

		EA_FORCE_INLINE void deallocate(void* p, size_t n) {
			allocator_->deallocate(p, n);
		}

		EA_FORCE_INLINE const char* get_name() const {
			return "Allocator Handle";
		}

		EA_FORCE_INLINE void set_name(const char*) {}

		EA_FORCE_INLINE bool operator==(AllocatorHandle const& rhs) const {
			return allocator_ == rhs.allocator_;
		}

		EA_FORCE_INLINE bool operator!=(AllocatorHandle const& rhs) const {
			return !(rhs == *this);
		}

	  private:
		Allocator* allocator_;
	};
	AllocatorHandle* default_allocator();
} // namespace bve::util
