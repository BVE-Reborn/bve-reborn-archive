#pragma once

#include "util/inlining_util.hpp"
#include "util/language.hpp"
#include <EASTL/allocator.h>
#include <cassert>
#include <foundational/allocation/tagged_pool.hpp>

namespace bve::util {
	// Allocator and AllocatorHandle in EASTL/allocator_fwd.hpp

	/**
	 * Linear allocator that pulls its memory from a \ref TaggedPool. Allocations are not freed at all, relying entirely on the pool's tag
	 * based deallocation to free up memory. If there isn't enough room in the allocation received from the pool, the rest of that
	 * allocation will be wasted as it will grab another one without using the rest of the current one.
	 *
	 * This class may be used with a standard container using a polymorphic allocator by passing the address as the last argument of the
	 * container's constructor.
	 *
	 * This class is not thread safe, but may be used from any thread in serial.
	 */
	class LinearAllocator final : public Allocator {
	  public:
		/**
		 * Initialize the allocator. Grabs one allocation from the upstream pool. Tag must not be zero.
		 *
		 * \param tagged_pool_ Pool to get backing memory from.
		 * \param tag_         Tag to associate with the memory.
		 */
		LinearAllocator(foundational::allocation::TaggedPool& tagged_pool_, std::uint8_t const tag_) :
		    tagged_pool(&tagged_pool_), allocation_size(tagged_pool_.get_allocation_size().b_count()), tag(tag_) {
			get_new_upstream_alloc();
		}
		/**
		 * Initialize the allocator. Grabs one allocation from the upstream pool. Uses arbitrary enum to represent the tag. Value of the
		 * enum must not be zero.
		 *
		 * \param tagged_pool_ Pool to get backing memory from.
		 * \param tag_         Tag to associate with the memory.
		 */
		template <class T, std::enable_if_t<std::is_enum_v<T>, void**> = nullptr>
		FOUNDATIONAL_FORCE_INLINE LinearAllocator(foundational::allocation::TaggedPool& tagged_pool_, T const tag_) :
		    LinearAllocator(tagged_pool_, static_cast<uint8_t>(tag_)) {
			static_assert(sizeof(T) == 1);
		}

		/**
		 * Allocate a chunk of memory. Not thread safe.
		 *
		 * \param size      Size of the allocation.
		 * \return          Pointer to your fresh new allocation.
		 */
		void* allocate(uSize size, int) override {
			assert(size <= allocation_size);

			round_to_alignment(16);
			uSize used_after_alloc = used + size;

			if (used_after_alloc >= allocation_size) {
				get_new_upstream_alloc();
				used_after_alloc = size;
			}

			void* ptr = static_cast<void*>(current_block + used);
			used = used_after_alloc;
			return ptr;
		}

		/**
		 * Allocate a chunk of memory. Not thread safe.
		 *
		 * \param size      Size of the allocation.
		 * \param alignment Alignment of the allocation. Must be 1, 2, 4, 8, or 16.
		 * \return          Pointer to your fresh new allocation.
		 */
		void* allocate(uSize size, uSize alignment, uSize, int) override {
			assert(size <= allocation_size);

			round_to_alignment(alignment);
			uSize used_after_alloc = used + size;

			if (used_after_alloc >= allocation_size) {
				get_new_upstream_alloc();
				used_after_alloc = size;
			}

			void* ptr = static_cast<void*>(current_block + used);
			used = used_after_alloc;
			return ptr;
		}

		void deallocate(void*, uSize) override {}

	  private:
		foundational::allocation::TaggedPool* tagged_pool;
		char* current_block;
		uSize allocation_size;
		uSize used{0};
		std::uint8_t tag;

		FOUNDATIONAL_FORCE_INLINE void round_to_alignment(uSize offset) {
			switch (offset) {
				case 1:
					break;
				case 2:
					used = (used + 1U) & ~0x1U;
					break;
				case 4:
					used = (used + 2U) & ~0b11U;
					break;
				case 8:
					used = (used + 4U) & ~0b111U;
					break;
				default:
				case 16:
					used = (used + 8U) & ~0b1111U;
					break;
			}
		}

		FOUNDATIONAL_FORCE_INLINE void get_new_upstream_alloc() {
			current_block = static_cast<char*>(tagged_pool->allocate(tag));
			used = 0;
		}
	};
} // namespace bve::util
