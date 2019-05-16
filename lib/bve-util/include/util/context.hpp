#pragma once

#include "util/allocators.hpp"
#include <foundational/allocation/linear_allocator.hpp>
#include <foundational/logging/logger.hpp>

namespace bve::util {
	struct Context {
		foundational::logging::Trace* trace;
		bve::util::LinearAllocator* alloc;
	};
} // namespace bve::util
