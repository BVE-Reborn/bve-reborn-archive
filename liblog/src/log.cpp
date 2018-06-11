#define LIBLOG_DEBUG
#include "log.hpp"
#include <fstream>
#include <gsl/gsl_util>
#include <mutex>
#include <vector>
logger::current_severity_container logger::current_severity;

logger::detail::current_time logger::detail::get_time() {
	auto now = std::chrono::system_clock::now();
	auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	auto s = std::chrono::system_clock::to_time_t(seconds);
	auto wall_time = std::gmtime(&s);
	auto year = wall_time->tm_year + 1900;
	auto month = wall_time->tm_mon;
	auto day = wall_time->tm_mday;
	auto hour = wall_time->tm_hour;
	auto min = wall_time->tm_min;
	auto sec = wall_time->tm_sec;
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);

	logger::detail::current_time current;
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
	std::unique_ptr<std::ostream> file_;
};

static Log log_impl;

void logger::to_log(std::string const& fmt_str) {
	std::lock_guard<std::mutex> lock(log_impl.file_lock_);
	*log_impl.file_ << fmt_str;
}
void logger::set_output_location(std::string const& name) {
	std::lock_guard<std::mutex> lock(log_impl.file_lock_);
	log_impl.file_ = std::make_unique<std::ofstream>(name, std::ios::app);
}
void logger::set_output_location(std::unique_ptr<std::ostream> name) {
	std::lock_guard<std::mutex> lock(log_impl.file_lock_);
	log_impl.file_ = std::move(name);
}

std::unique_ptr<std::ostream> get_output_stream() {
	return std::move(log_impl.file_);
}

void func() {
	LOG(fatal_error, "{}", "hello");
}