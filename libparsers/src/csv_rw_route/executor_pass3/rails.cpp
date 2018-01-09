#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::RailStart& instr) {}
	void pass3_executor::operator()(const instructions::track::Rail& instr) {}
	void pass3_executor::operator()(const instructions::track::RailType& instr) {}
	void pass3_executor::operator()(const instructions::track::RailEnd& instr) {}
	void pass3_executor::operator()(const instructions::track::Adhesion& instr) {}
} // namespace csv_rw_route
} // namespace parsers
