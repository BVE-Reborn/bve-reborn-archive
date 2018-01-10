#include "../executor_pass3.hpp"
#include <ostream>
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void print_cycle_type(std::ostream& o, const cycle_type& c) {
		auto print_regular_filename = [&o](const filename_set_iterator& i) { o << *i << '\n'; };
		auto print_cycle_filename = [&o](const std::vector<std::size_t>& list) {
			o << "Cycle of: (";
			std::size_t i = 0;
			for (auto& item : list) {
				o << "#" << item;
				if (++i != list.size()) {
					o << ", ";
				}
				o << ")";
			}
		};

		c.match(print_regular_filename, print_cycle_filename);
	}

	void pass3_executor::operator()(const instructions::cycle::Ground& inst) {
		std::vector<std::size_t> cycle;

		for (auto& ground_index : inst.input_indices) {
			cycle.emplace_back(ground_index);
		}

		auto insert_ret = object_ground_mapping.insert({inst.ground_structure_index, cycle});

		auto& iterator = insert_ret.first;
		auto& inserted = insert_ret.second;

		if (!inserted) {
			auto old_value = iterator->second;
			iterator->second = cycle;

			std::ostringstream err;

			err << "Cycle.Ground overwriting index number " << inst.ground_structure_index << ". Old Filename: \"";
			print_cycle_type(err, old_value);
			err << "\". Current Filename: \"";
			print_cycle_type(err, cycle);
			err << "\".";

			_errors[get_filename(inst.file_index)].emplace_back<errors::error_t>({inst.line, err.str()});
		}
	}

	void pass3_executor::operator()(const instructions::cycle::Rail& inst) {
		std::vector<std::size_t> cycle;

		for (auto& ground_index : inst.input_indices) {
			cycle.emplace_back(ground_index);
		}

		auto insert_ret = object_rail_mapping.insert({inst.rail_structure_index, cycle});

		auto& iterator = insert_ret.first;
		auto& inserted = insert_ret.second;

		if (!inserted) {
			auto old_value = iterator->second;
			iterator->second = cycle;

			std::ostringstream err;

			err << "Cycle.Rail overwriting index #" << inst.rail_structure_index << ". Old Filename: \"";
			print_cycle_type(err, old_value);
			err << "\". Current Filename: \"";
			print_cycle_type(err, cycle);
			err << "\".";

			_errors[get_filename(inst.file_index)].emplace_back<errors::error_t>({inst.line, err.str()});
		}
	}
} // namespace csv_rw_route
} // namespace parsers
