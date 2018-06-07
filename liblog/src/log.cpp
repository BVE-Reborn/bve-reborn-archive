#include "log.hpp"
log::current_severity_container log::current_severity;

log::detail::current_time log::detail::get_time() {
	using Clock = std::chrono::system_clock;

	auto now = Clock::now();
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto s = Clock::to_time_t(seconds);
	auto wall_time = std::gmtime(&s);
	auto year = wall_time->tm_year + 1900;
	auto month = wall_time->tm_mon;
	auto day = wall_time->tm_mday;
	auto hour = wall_time->tm_hour;
	auto min = wall_time->tm_min;
	auto sec = wall_time->tm_sec;
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);

}