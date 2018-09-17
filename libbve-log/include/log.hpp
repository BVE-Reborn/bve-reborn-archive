#pragma once

#include "core/inlining_util.hpp"
#include "core/macro_helpers.hpp"
#include <atomic>
#include <chrono>
#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include <type_traits>

/**
 * \ingroup liblog
 * \def LIBLOG_DEBUG
 * \brief Enables \link logger::severity::debug severity::debug \endlink logging messages and adds debug information to logging output.
 */

/**
 * \ingroup liblog
 * \def LIBLOG_NO_SIMPLE_MACRO_NAMES
 * \brief Disables the \ref LOG macro, forcing use of \ref LIBLOG_LOG.
 */
#ifdef DOXYGEN
#	define LIBLOG_DEBUG
#	define LIBLOG_NO_SIMPLE_MACRO_NAMES
#	undef LIBLOG_NO_SIMPLE_MACRO_NAMES
#endif

namespace logger {

	/**
	 * \brief Sets severity of message for log filtering.
	 */
	enum class severity {
		/// \brief For extremely verbose debugging information. Requires \ref LIBLOG_DEBUG <b>Default: Hidden</b>
		debug,
		/// \brief Unimportant information. <b>Default: Hidden</b>
		info,
		/// \brief Can be used to describe the process that caused a warning. <b>Default: Shown</b>
		note,
		/// \brief Something is probably not doing what it was intended for. <b>Default: Shown</b>
		warning,
		/// \brief There is a glaring problem that can be worked around, but poorly. <b>Default: Shown</b>
		error,
		/// \brief The program cannot recover, the operation must be terminated. <b>Default: Shown</b>
		fatal_error
	};

	namespace detail {
		/**
		 * \brief Helper for the integer type underlying \ref logger::severity
		 */
		using severity_int_type = std::underlying_type_t<severity>;

		/**
		 * \brief Holds the current time, accurate down to the millisecond.
		 * \details We don't use any of the builtin time types because they lack support for accuracy down to the millisecond.
		 */
		struct current_time {
			int year;
			int month;
			int day;
			int hour;
			int minute;
			int second;
			int millisecond;
		};

		/**
		 * \brief Gets the current time, accurate to the millisecond
		 * \return Current time
		 */
		current_time get_time();

		/**
		 * \brief Atomic container for the current severity.
		 */
		class current_severity_container {
		  public:
			/**
			 * \brief Initializes the container with the default severity of \ref severity::note
			 */
			current_severity_container() : current_sev_{severity::note} {}
			/**
			 * @brief Atomically gets the current severity
			 * @return Current Severity
			 */
			severity get() {
				return current_sev_;
			}
			/**
			 * \brief Atomically sets global severity
			 * \param new_sev New severity value.
			 */
			void set(severity new_sev) {
				current_sev_ = new_sev;
			}

		  private:
			std::atomic<severity> current_sev_;
		};
	} // namespace detail

	/**
	 * \brief Global object that marks the current severity
	 */
	inline detail::current_severity_container current_severity;

	/**
	 * \ingroup liblog
	 * \brief Sends preformatted text to the log. You should not be using this method directly. Use the \ref LOG macro instead.
	 * \details This will block on the file or object used internally for synchronization.
	 * \param fmt_str Preformatted text.
	 */
	void to_log(std::string const& fmt_str);

	/**
	 * \ingroup liblog
	 * \brief Chooses the current allowed severity level for messages. Any message at or above it is allowed through.
	 * \param s Severity to set.
	 */
	FORCE_INLINE void set_visible_severity(severity s) {
		current_severity.set(s);
	}

	/**
	 * \ingroup liblog
	 * \brief Sets the log output location to a filename.
	 * \details An ostream will be constructed internally to send to the file.
	 *
	 * \param name Relative filename log will be sent to. File will be open in append mode.
	 */
	void set_output_location(std::string const& name);

	/**
	 * \ingroup liblog
	 * \brief Sets the log output location to an ostream.
	 * \details Shared pointer allows you to get the handle back which can be useful for testing and information gathering.
	 * \param stream The ostream object the log file should write to.
	 */
	void set_output_location(std::shared_ptr<std::ostream> stream);

	/**
	 * \ingroup liblog
	 * \brief Returns a shared pointer to the ostream object the logger is writing to.
	 */
	std::shared_ptr<std::ostream> get_output_stream();

} // namespace logger

#ifdef LIBLOG_DEBUG
#	define LIBLOG_FORMAT_CALL(ser, fmt_str, ...)                                                  \
		::fmt::format(fmt("{:0>4d}.{:0>2d}.{:0>2d} {:0>2d}:{:0>2d}:{:0>2d}.{:0>3d}: {:s}: "        \
		                  "{:s}:{:d}: " fmt_str "\n"),                                             \
		              time.year, time.month, time.day, time.hour, time.minute, time.second,        \
		              time.millisecond, #ser, __FILE__, __LINE__, __VA_ARGS__)
#else
#	define LIBLOG_FORMAT_CALL(ser, fmt_str, ...)                                                  \
		::fmt::format(fmt("{:0>4d}.{:0>2d}.{:0>2d} {:0>2d}:{:0>2d}:{:0>2d}.{:0>3d}: "              \
		                  "{:s}: " fmt_str "\n"),                                                  \
		              time.year, time.month, time.day, time.hour, time.minute, time.second,        \
		              time.millisecond, #ser, __VA_ARGS__)
#endif

#define LIBLOG_LOG_SEVERITY_IMPL(ser, fmt_str, ...)                                                \
	{                                                                                              \
		auto const time = ::logger::detail::get_time();                                            \
		auto const formated_str = EXPAND(LIBLOG_FORMAT_CALL(ser, fmt_str, __VA_ARGS__));           \
		::logger::to_log(std::move(formated_str));                                                 \
	}
#ifdef LIBLOG_DEBUG
#	define LIBLOG_LOG_SEVERITY_debug(...)                                                         \
		if (static_cast<::logger::detail::severity_int_type>(::log::current_severity.get())        \
		    <= static_cast<::logger::detail::severity_int_type>(::log::severity::debug)) {         \
			EXPAND(LIBLOG_LOG_SEVERITY_IMPL(debug, __VA_ARGS__))                                   \
		}
#else
#	define LIBLOG_LOG_SEVERITY_debug(...)
#endif
#define LIBLOG_LOG_SEVERITY_info(...)                                                              \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    <= static_cast<::logger::detail::severity_int_type>(::logger::severity::info)) {           \
		EXPAND(LIBLOG_LOG_SEVERITY_IMPL(info, __VA_ARGS__))                                        \
	}
#define LIBLOG_LOG_SEVERITY_note(...)                                                              \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    <= static_cast<::logger::detail::severity_int_type>(::logger::severity::note)) {           \
		EXPAND(LIBLOG_LOG_SEVERITY_IMPL(note, __VA_ARGS__))                                        \
	}
#define LIBLOG_LOG_SEVERITY_warning(...)                                                           \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    <= static_cast<::logger::detail::severity_int_type>(::logger::severity::warning)) {        \
		EXPAND(LIBLOG_LOG_SEVERITY_IMPL(warning, __VA_ARGS__))                                     \
	}
#define LIBLOG_LOG_SEVERITY_error(...)                                                             \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    <= static_cast<::logger::detail::severity_int_type>(::logger::severity::error)) {          \
		EXPAND(LIBLOG_LOG_SEVERITY_IMPL(error, __VA_ARGS__))                                       \
	}
#define LIBLOG_LOG_SEVERITY_fatal_error(...)                                                       \
	if (static_cast<::logger::detail::severity_int_type>(::logger::current_severity.get())         \
	    <= static_cast<::logger::detail::severity_int_type>(::logger::severity::fatal_error)) {    \
		EXPAND(LIBLOG_LOG_SEVERITY_IMPL(fatal_error, __VA_ARGS__))                                 \
	}
/**
 * \ingroup liblog
 * \brief Formats given string and writes it to the log.
 * \param sev <b>Unqualified</b> member of \ref logger::severity to set for this message.
 * \param format_str Format string that will be passed directly to fmt.
 * \param ... Arguments to be added into the format string.
 */
#define LIBLOG_LOG(sev, format_str, ...)                                                           \
	EXPAND(CONCAT(LIBLOG_LOG_SEVERITY_, sev)(format_str, __VA_ARGS__))

#ifndef LIBLOG_NO_SIMPLE_MACRO_NAMES
/**
 * \ingroup liblog
 * \copydoc LIBLOG_LOG
 */
#	define LOG(sev, format_str, ...) EXPAND(LIBLOG_LOG(sev, format_str, __VA_ARGS__))
#endif
