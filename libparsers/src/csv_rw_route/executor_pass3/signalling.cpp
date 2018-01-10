#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Limit& inst) {}
	void pass3_executor::operator()(const instructions::track::Section& inst) {}
	void pass3_executor::operator()(const instructions::track::SigF& inst) {}
	void pass3_executor::operator()(const instructions::track::Signal& inst) {}
	void pass3_executor::operator()(const instructions::track::Relay& inst) {}
} // namespace csv_rw_route
} // namespace parsers
