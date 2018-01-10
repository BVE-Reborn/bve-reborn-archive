#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Beacon& inst) {}
	void pass3_executor::operator()(const instructions::track::Transponder& inst) {}
	void pass3_executor::operator()(const instructions::track::Pattern& inst) {}
} // namespace csv_rw_route
} // namespace parsers
