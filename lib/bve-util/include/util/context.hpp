#pragma once

#include <foundational/allocation/linear_allocator.hpp>
#include <foundational/logging/logger.hpp>

namespace bve::util {
	struct Context {
		foundational::logging::Trace* trace;
		foundational::allocation::LinearAllocator* alloc;
	};
} // namespace bve::util
