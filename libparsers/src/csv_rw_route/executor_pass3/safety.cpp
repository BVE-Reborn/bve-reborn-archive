#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Beacon& instr) {}
	void pass3_executor::operator()(const instructions::track::Transponder& instr) {}
	void pass3_executor::operator()(const instructions::track::Pattern& instr) {}
} // namespace csv_rw_route
} // namespace parsers
