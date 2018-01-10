#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Sta& inst) {}
	void pass3_executor::operator()(const instructions::track::Stop& inst) {}
	void pass3_executor::operator()(const instructions::track::Form& inst) {}
} // namespace csv_rw_route
} // namespace parsers
