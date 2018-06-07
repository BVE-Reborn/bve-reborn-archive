#include "log.hpp"
#include <fstream>
#include <gsl/gsl_util>
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

class Log{
public:
    void log(std::string const& fmt_str){
        std::ofstream file(output_location_.c_str(), std::ios::app);
        file.write(fmt_str.c_str(), fmt_str.size());
        file.close();
    }
    void set_output_location(std::string const& location){
        output_location_ = location;
    }

private:
    std::string output_location_ = "Errors.txt";
};

