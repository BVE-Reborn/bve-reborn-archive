#include <core/threading/scheduler.hpp>
#include <doctest.h>

TEST_SUITE_BEGIN("libcore - threading");

using bve::core::threading::TaskScheduler;

TEST_CASE("libcore - threading - scheduler - sum") {
	TaskScheduler ts;

	auto dependent = ts.prepareDependentTask([](TaskScheduler& ts) {
		ts.stop(); // Separate for debugging
	});

	constexpr std::size_t count = 100;

	std::array<std::size_t, count> partials;

	for (std::size_t i = 0; i < count; ++i) {
		ts.addTask(
		    [i, &partials](TaskScheduler&) {
			    partials[i] = i; // Separate for debugging
		    },
		    dependent);
	}

	ts.addTask(std::move(dependent));

	ts.run(2, 0);

	for (std::size_t i = 0; i < count; ++i) {
		CHECK_EQ(partials[i], i);
	}
}

TEST_CASE("libcore - threading - scheduler - nested sum") {
	TaskScheduler ts;

	constexpr std::size_t count = 100;

	std::atomic<std::size_t> counter = 0;
	std::array<std::size_t, count * 2> partials;

	for (std::size_t i = 0; i < count; ++i) {
		ts.addTask([i, &partials, &counter](TaskScheduler& ts) {
			partials[i] = i; // Separate for debugging
			ts.addTask([i, &partials, &counter](TaskScheduler& ts) {
				partials[i + 100] = i + 100;
				if (counter.fetch_add(1, std::memory_order_acq_rel) == 99) {
					ts.stop();
				}
			});
		});
	}

	ts.run(2, 0);

	for (std::size_t i = 0; i < count * 2; ++i) {
		CHECK_EQ(partials[i], i);
	}
}

TEST_SUITE_END();
