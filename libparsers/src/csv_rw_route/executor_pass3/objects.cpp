#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::FreeObj& instr) {}
	void pass3_executor::operator()(const instructions::track::Wall& instr) {}
	void pass3_executor::operator()(const instructions::track::WallEnd& instr) {}
	void pass3_executor::operator()(const instructions::track::Dike& instr) {}
	void pass3_executor::operator()(const instructions::track::DikeEnd& instr) {}
	void pass3_executor::operator()(const instructions::track::Pole& instr) {}
	void pass3_executor::operator()(const instructions::track::PoleEnd& instr) {}
	void pass3_executor::operator()(const instructions::track::Crack& instr) {}
	void pass3_executor::operator()(const instructions::track::Ground& instr) {}
} // namespace csv_rw_route
} // namespace parsers
