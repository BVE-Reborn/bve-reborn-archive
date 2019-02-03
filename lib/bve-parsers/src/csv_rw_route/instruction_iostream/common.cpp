#include "common.hpp"

std::ostream& operator<<(std::ostream& os, const bve::parsers::csv_rw_route::Instruction& i) {
	os << std::boolalpha;

	bve::parsers::csv_rw_route::CSVRwRouteInstructionIOClass io_class(os);

	apply_visitor(io_class, i);

	return os;
}

std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::csv_rw_route::InstructionList& list) {
	os << "Files:\n";
	for (std::size_t i = 0; i < list.filenames.size(); ++i) {
		os << i << ": " << list.filenames[i] << '\n';
	}
	os << "Instructions:\n";
	for (auto& i : list.instructions) {
		os << std::boolalpha;

		bve::parsers::csv_rw_route::CSVRwRouteInstructionIOClass io_class(os, list.filenames);

		apply_visitor(io_class, i);

		os << "\n";
	}
	return os;
}
