#pragma once

#include <EASTL/allocator.h>
#include <foundational/util/inlining_util.hpp>

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
		FOUNDATIONAL_FORCE_INLINE AllocatorHandle(Allocator* allocator) : allocator_(allocator) {}

        FOUNDATIONAL_FORCE_INLINE AllocatorHandle(const AllocatorHandle& x) = default;

        FOUNDATIONAL_FORCE_INLINE ~AllocatorHandle() = default;

        FOUNDATIONAL_FORCE_INLINE AllocatorHandle& operator=(const AllocatorHandle& x) = default;

        FOUNDATIONAL_FORCE_INLINE void* allocate(size_t n, int flags = 0) {
			return allocator_->allocate(n, flags);
		}

        FOUNDATIONAL_FORCE_INLINE void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0) {
			return allocator_->allocate(n, alignment, offset, flags);
		}

        FOUNDATIONAL_FORCE_INLINE void deallocate(void* p, size_t n) {
			allocator_->deallocate(p, n);
		}

        FOUNDATIONAL_FORCE_INLINE const char* get_name() const {
			return "Allocator Handle";
		}

        FOUNDATIONAL_FORCE_INLINE void set_name(const char*) {
		}

        FOUNDATIONAL_FORCE_INLINE bool operator==(AllocatorHandle const& rhs) const {
            return allocator_ == rhs.allocator_;
        }

        FOUNDATIONAL_FORCE_INLINE bool operator!=(AllocatorHandle const& rhs) const {
            return !(rhs == *this);
        }

    private:
		Allocator* allocator_;
	};
} // namespace bve::util
