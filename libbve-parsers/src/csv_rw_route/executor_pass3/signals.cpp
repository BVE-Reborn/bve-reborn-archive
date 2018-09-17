#include "executor_pass3.hpp"
#include <sstream>

namespace parsers::csv_rw_route {
	void pass3_executor::operator()(const instructions::naked::signal_animated& inst) {
		auto this_signal = animated_signal{inst.filename};
		auto insert_pair = std::make_pair(inst.signal_index, this_signal);

		auto const insert_ret = signal_mapping_.insert(insert_pair);

		auto iterator = insert_ret.first;
		auto const inserted = insert_ret.second;

		if (!inserted) {
			auto old_value = iterator->second;
			iterator->second = this_signal;

			std::ostringstream err;
			err << "Signal(Animated) is overwriting signal at index (" << inst.signal_index
			    << "). Old Value: ";
			old_value.match([&err](const animated_signal& as)
			                    -> void { err << "\"" << as.filename << "\"\n"; },
			                [&err](const traditional_signal& ts) -> void {
				                err << "(\"" << ts.glow_filename << ".{x,csv,b3d}\", \""
				                    << ts.glow_filename << ".{bmp,png,jpg}\")";
			                });
			err << ". New Value: \"" << inst.filename << "\".";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}
	}

	void pass3_executor::operator()(const instructions::naked::signal& inst) {
		auto this_signal = traditional_signal{inst.signal_filename, inst.glow_filename};

		auto insert_pair = std::make_pair(inst.signal_index, this_signal);

		auto const insert_ret = signal_mapping_.insert(insert_pair);

		auto iterator = insert_ret.first;
		auto const inserted = insert_ret.second;

		if (!inserted) {
			auto old_value = iterator->second;
			iterator->second = this_signal;

			std::ostringstream err;

			auto print_animated_file = [&err](const animated_signal& as) -> void {
				err << "\"" << as.filename << "\"\n";
			};
			auto print_traditional_tuple = [&err](const traditional_signal& ts) -> void {
				err << "(\"" << ts.glow_filename << ".{x,csv,b3d}\", \"" << ts.glow_filename
				    << ".{bmp,png,jpg}\")";
			};

			err << "Signal(Traditional) is overwriting signal at index (" << inst.signal_index
			    << "). Old Value: ";
			old_value.match(print_animated_file, print_traditional_tuple);
			err << "New Value: ";
			print_traditional_tuple(this_signal);
			err << ".";
			add_error(errors_, get_filename(inst.file_index), inst.line, err);
		}
	}
} // namespace parsers::csv_rw_route
