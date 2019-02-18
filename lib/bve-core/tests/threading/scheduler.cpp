#include <core/threading/scheduler.hpp>
#include <doctest.h>

TEST_SUITE_BEGIN("libcore - threading");

using bve::core::threading::TaskScheduler;

TEST_CASE("libcore - threading - scheduler - sum") {
	TaskScheduler ts(2, 0);

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

	ts.addDependentTask(std::move(dependent));

	ts.run();

	for (std::size_t i = 0; i < count; ++i) {
		CHECK_EQ(partials[i], i);
	}
}

TEST_CASE("libcore - threading - scheduler - nested sum") {
	TaskScheduler ts(2, 0);

	constexpr std::size_t count = 100;

	std::atomic<std::size_t> counter = 0;
	std::array<std::size_t, count * 2> partials;

	// First task increments does 0->99
	// Inner task does 100->199
	// When counter increment returns 99 all tasks are done.
	for (std::size_t i = 0; i < count; ++i) {
		ts.addTask([i, count, &partials, &counter](TaskScheduler& ts) {
			partials[i] = i; // Separate for debugging
			ts.addTask([i, count, &partials, &counter](TaskScheduler& ts) {
				partials[i + count] = i + count;
				if (counter.fetch_add(1, std::memory_order_acq_rel) == count - 1) {
					ts.stop();
				}
			});
		});
	}

	ts.run();

	for (std::size_t i = 0; i < count * 2; ++i) {
		CHECK_EQ(partials[i], i);
	}
}

TEST_CASE("libcore - threading - scheduler - double dependants") {
	TaskScheduler ts(2, 0);

	std::size_t counter = 0;

	auto dep2 = ts.prepareDependentTask([&counter](TaskScheduler& ts) {
		counter *= 2;
		ts.stop();
	});
	auto dep1 = ts.prepareDependentTask(
	    [&counter](auto&) {
		    counter = 4; // for debugging
	    },
	    dep2);
	ts.addTask([&counter](auto&) { counter = 1; }, dep1);

	ts.addDependentTask(std::move(dep1));
	ts.addDependentTask(std::move(dep2));

	ts.run();

	CHECK_EQ(counter, 8);
}

TEST_CASE("libcore - threading - scheduler - blocking double dependants") {
	TaskScheduler ts(1, 2);

	std::size_t counter = 0;

	auto dep2 = ts.prepareDependentBlockingTask([&counter](TaskScheduler& ts) {
		counter *= 2;
		ts.stop();
	});
	auto dep1 = ts.prepareDependentBlockingTask(
		[&counter](auto&) {
		counter = 4; // for debugging
	},
		dep2);
	ts.addBlockingTask([&counter](auto&) { counter = 1; }, dep1);

	ts.addDependentTask(std::move(dep1));
	ts.addDependentTask(std::move(dep2));

	ts.run();

	CHECK_EQ(counter, 8);
}

TEST_CASE("libcore - threading - scheduler - tasks run after pause") {
	TaskScheduler ts(2, 0);

	std::size_t counter = 0;

	auto set_counter = ts.prepareDependentTask([&counter](TaskScheduler& ts) { counter = 1; ts.stop(); });
	ts.addTask([](TaskScheduler& ts) { ts.pause(); }, set_counter);
	ts.addDependentTask(std::move(set_counter));

	ts.run();

	CHECK_EQ(counter, 0);

	ts.run();

	CHECK_EQ(counter, 1);
}

TEST_SUITE_END();
