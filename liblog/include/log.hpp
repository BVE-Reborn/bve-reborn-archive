#pragma once
#include <atomic>
#include <chrono>
#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include <type_traits>
namespace logger {

enum class severity { debug, info, note, warning, error, fatal_error };

namespace detail {
	using severity_int_type = std::underlying_type_t<severity>;
	struct current_time {
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		int millisecond;
	};
	current_time get_time();
} // namespace detail

class current_severity_container {
  public:
	current_severity_container() : current_sev_{severity::note} {}
	severity get() {
		return current_sev_;
	}
	void set(severity new_sev) {
		current_sev_ = new_sev;
	}

  private:
	std::atomic<severity> current_sev_;
};

extern current_severity_container current_severity;

void to_log(std::string const& s);

void set_output_location(std::string const& name);

void set_output_location(std::shared_ptr<std::ostream> name);

std::shared_ptr<std::ostream> get_output_stream();
} // namespace logger

#ifdef LIBLOG_DEBUG
#	define LIBLOG_FORMAT_CALL(ser, fmt_str, ...)                                                  \
		fmt::format("{:0>4d}.{:0>2d}.{:0>2d} {:0>2d}:{:0>2d}:{:0>2d}.{:0>3d}: {s}: {s}:{d}:" fmt_str, time.year, time.month,        \
		            time.day, time.hour, time.minute, time.second, time.millisecond, #ser,         \
		            __FILE__, __LINE__, __VA_ARGS__)
#else
#	define LIBLOG_FORMAT_CALL(ser, fmt_str, ...)                                                  \
		fmt::format("{:0>4d}.{:0>2d}.{:0>2d} {:0>2d}:{:0>2d}:{:0>2d}.{:0>3d}: {s}: " fmt_str, time.year, time.month, time.day,    \
		            time.hour, time.minute, time.second, time.millisecond, #ser, __VA_ARGS__)
#endif

#define LIBLOG_LOG_SEVERITY_IMPL(ser, fmt_str, ...)                                                \
	{                                                                                              \
		auto const time = ::logger::detail::get_time();                                            \
		auto const formated_str = LIBLOG_FORMAT_CALL(ser, fmt_str, __VA_ARGS__);                   \
		::logger::to_log(std::move(formated_str));                                                 \
	}
#ifdef LIBLOG_DEBUG
#	define LIBLOG_LOG_SEVERITY_debug(...)                                                         \
		if (static_cast<::logger::detail::severity_int_type>(::log::current_severity.get())        \
		    == static_cast<::logger::detail::severity_int_type>(::log::severity::debug)) {         \
			LIBLOG_LOG_SEVERITY_IMPL(debug, __VA_ARGS__)                                           \
		}
#else
#	define LIBLOG_LOG_SEVERITY_debug(...)
#endif
#define LIBLOG_LOG_SEVERITY_info(...)                                                              \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    == static_cast<::logger::detail::severity_int_type>(::logger::severity::info)) {           \
		LIBLOG_LOG_SEVERITY_IMPL(info, __VA_ARGS__)                                                \
	}
#define LIBLOG_LOG_SEVERITY_note(...)                                                              \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    == static_cast<::logger::detail::severity_int_type>(::logger::severity::note)) {           \
		LIBLOG_LOG_SEVERITY_IMPL(note, __VA_ARGS__)                                                \
	}
#define LIBLOG_LOG_SEVERITY_warning(...)                                                           \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    == static_cast<::logger::detail::severity_int_type>(::logger::severity::warning)) {        \
		LIBLOG_LOG_SEVERITY_IMPL(warning, __VA_ARGS__)                                             \
	}
#define LIBLOG_LOG_SEVERITY_error(...)                                                             \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    == static_cast<::logger::detail::severity_int_type>(::logger::severity::error)) {          \
		LIBLOG_LOG_SEVERITY_IMPL(error, __VA_ARGS__)                                               \
	}
#define LIBLOG_LOG_SEVERITY_fatal_error(...)                                                       \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    == static_cast<::logger::detail::severity_int_type>(::logger::severity::fatal_error)) {    \
		LIBLOG_LOG_SEVERITY_IMPL(fatal_error, __VA_ARGS__)                                         \
	}
#define LIBLOG_LOG(sev, ...) LIBLOG_LOG_SEVERITY_##sev(__VA_ARGS__)

#ifndef LIBLOG_NO_SIMPLE_MACRO_NAMES
#	define LOG(...) LIBLOG_LOG(__VA_ARGS__)
#endif
