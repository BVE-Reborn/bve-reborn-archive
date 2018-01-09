#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Sta& instr) {}
	void pass3_executor::operator()(const instructions::track::Stop& instr) {}
	void pass3_executor::operator()(const instructions::track::Form& instr) {}
} // namespace csv_rw_route
} // namespace parsers
