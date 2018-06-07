#pragma once
#include "time.h"
#include <chrono>
#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include <type_traits>
namespace log {

enum class severity { debug, info, note, warning, error, fatal_error };

namespace detail {
	using severity_int_type = std::underlying_type_t<severity>;
	struct current_time {
		int year;
		int month;
		int day;
		int hour;
		int minuete;
		int second;
		int milisecond;
	};
	current_time get_time();
} // namespace detail

class current_severity_container {
  public:
	severity get() {
		return current_sev_;
	}
	void set(severity new_sev) {
		current_sev_ = new_sev;
	}

  private:
	severity current_sev_ = severity::note;
};

extern current_severity_container current_severity;

void to_log(std::string const& s);

void set_output_location(std::string const& name);

void set_output_location(std::unique_ptr<std::ostream> name);
} // namespace log

#ifdef LIBLOG_DEBUG
#	define LIBLOG_FORMAT_CALL(ser, fmt_str, ...)                                                  \
		fmt::format(fmt("{}.{}.{} {}:{}:{}.{}: {}: {}:{}:" fmt_str), __VA_ARGS__)
#else
#	define LIBLOG_FORMAT_CALL(ser, fmt_str, ...)                                                  \
		fmt::format(fmt("{}.{}.{} {}:{}:{}.{}: {}: " fmt_str), __VA_ARGS__)
#endif

#define LIBLOG_LOG_SEVERITY_IMPL(ser, fmt_str, ...)                                                \
	auto formated_str = LIBLOG_FORMAT_CALL(ser, fmt_str, __VA_ARGS__);
#ifdef LIBLOG_DEBUG
#	define LIBLOG_LOG_SEVERITY_debug(...)                                                         \
		if (static_cast<::detail::severity_int_type>(::log::current_severity.get())                \
		    == static_cast<::detail::severity_int_type>(::log::severity::debug)) {                 \
			LIBLOG_LOG_SEVERITY_IMPL(debug, __VA_ARGS__)                                           \
		}
#else
#	define LIBLOG_LOG_SEVERITY_debug(...)
#endif
#define LIBLOG_LOG_SEVERITY_info(...)                                                              \
	if (static_cast<::detail::severity_int_type>(::log::current_severity.get())                    \
	    == static_cast<::detail::severity_int_type>(::log::severity::info)) {                      \
		LIBLOG_LOG_SEVERITY_IMPL(info, __VA_ARGS__)                                                \
	}
#define LIBLOG_LOG_SEVERITY_note(...)                                                              \
	if (static_cast<::detail::severity_int_type>(::log::current_severity.get())                    \
	    == static_cast<::detail::severity_int_type>(::log::severity::note)) {                      \
		LIBLOG_LOG_SEVERITY_IMPL(note, __VA_ARGS__)                                                \
	}
#define LIBLOG_LOG_SEVERITY_warning(...)                                                           \
	if (static_cast<::detail::severity_int_type>(::log::current_severity.get())                    \
	    == static_cast<::detail::severity_int_type>(::log::severity::warning)) {                   \
		LIBLOG_LOG_SEVERITY_IMPL(warning, __VA_ARGS__)                                             \
	}
#define LIBLOG_LOG_SEVERITY_error(...)                                                             \
	if (static_cast<::detail::severity_int_type>(::log::current_severity.get())                    \
	    == static_cast<::detail::severity_int_type>(::log::severity::error)) {                     \
		LIBLOG_LOG_SEVERITY_IMPL(error, __VA_ARGS__)                                               \
	}
#define LIBLOG_LOG_SEVERITY_fatal_error(...)                                                       \
	if (static_cast<::detail::severity_int_type>(::log::current_severity.get())                    \
	    == static_cast<::detail::severity_int_type>(::log::severity::fatal_error)) {               \
		LIBLOG_LOG_SEVERITY_IMPL(fatal_error, __VA_ARGS__)                                         \
	}
#define LIBLOG_LOG(serv, ...) _LIBLOG_LOG_SEVERITY_##sev(__VA_ARGS__)
