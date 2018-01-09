#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Limit& instr) {}
	void pass3_executor::operator()(const instructions::track::Section& instr) {}
	void pass3_executor::operator()(const instructions::track::SigF& instr) {}
	void pass3_executor::operator()(const instructions::track::Signal& instr) {}
	void pass3_executor::operator()(const instructions::track::Relay& instr) {}
} // namespace csv_rw_route
} // namespace parsers
