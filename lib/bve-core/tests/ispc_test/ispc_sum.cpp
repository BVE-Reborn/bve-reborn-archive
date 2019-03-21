#include "ispc/ispc_sum.h"
#include <algorithm>
#include <doctest/doctest.h>
#include <vector>

TEST_SUITE_BEGIN("libcore - ispc");

TEST_CASE("libcore - ispc") {
	std::vector<uint64_t> array;

	constexpr std::size_t size = 10000;

	uint64_t i = 0;
	std::generate_n(std::back_inserter(array), size + 1, [&] { return i++; });

	auto sum = ispc::sum(array.data(), array.size());

	CHECK_EQ(sum, (size * (size + 1)) / 2);
}
