#include "log.hpp"
#include <fstream>
#include <gsl/gsl_util>
#include <mutex>
#include <time.h>

bve::log::detail::current_time bve::log::detail::get_time() {
	auto const now = std::chrono::system_clock::now();
	auto const seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto s = std::chrono::system_clock::to_time_t(seconds);
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
	tm wall_time;
	gmtime_r(&s, &wall_time);
#else
	tm wall_time;
	gmtime_s(&wall_time, &s);
#endif
	auto const year = wall_time.tm_year + 1900;
	auto const month = wall_time.tm_mon;
	auto const day = wall_time.tm_mday;
	auto const hour = wall_time.tm_hour;
	auto const min = wall_time.tm_min;
	auto const sec = wall_time.tm_sec;
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);

	log::detail::current_time current;
	current.year = year;
	current.month = month;
	current.day = day;
	current.hour = hour;
	current.minute = min;
	current.second = sec;
	current.millisecond = gsl::narrow_cast<int>(milli.count());
	return current;
}

struct Log {
	std::mutex file_lock_;
	std::shared_ptr<std::ostream> file_;
};

static Log log_impl;

void bve::log::to_log(std::string const& fmt_str) {
	std::lock_guard<std::mutex> lock(log_impl.file_lock_);
	*log_impl.file_ << fmt_str;
}
void bve::log::set_output_location(std::string const& name) {
	std::lock_guard<std::mutex> lock(log_impl.file_lock_);
	log_impl.file_ = std::make_shared<std::ofstream>(name, std::ios::app);
}
void bve::log::set_output_location(std::shared_ptr<std::ostream> stream) {
	std::lock_guard<std::mutex> lock(log_impl.file_lock_);
	log_impl.file_ = std::move(stream);
}

std::shared_ptr<std::ostream> bve::log::get_output_stream() {
	return log_impl.file_;
}
