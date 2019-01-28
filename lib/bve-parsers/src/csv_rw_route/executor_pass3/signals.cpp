#include "executor_pass3.hpp"
#include <sstream>

namespace bve::parsers::csv_rw_route {
	void Pass3Executor::operator()(const instructions::naked::SignalAnimated& inst) {
		auto this_signal = AnimatedSignal{inst.filename};
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
			old_value.match([&err](const AnimatedSignal& as)
			                    -> void { err << "\"" << as.filename << "\"\n"; },
			                [&err](const TraditionalSignal& ts) -> void {
				                err << "(\"" << ts.glow_filename << ".{x,csv,b3d}\", \""
				                    << ts.glow_filename << ".{bmp,png,jpg}\")";
			                });
			err << ". New Value: \"" << inst.filename << "\".";
			add_error(errors_, getFilename(inst.file_index), inst.line, err);
		}
	}

	void Pass3Executor::operator()(const instructions::naked::Signal& inst) {
		auto this_signal = TraditionalSignal{inst.signal_filename, inst.glow_filename};

		auto insert_pair = std::make_pair(inst.signal_index, this_signal);

		auto const insert_ret = signal_mapping_.insert(insert_pair);

		auto iterator = insert_ret.first;
		auto const inserted = insert_ret.second;

		if (!inserted) {
			auto old_value = iterator->second;
			iterator->second = this_signal;

			std::ostringstream err;

			auto print_animated_file = [&err](const AnimatedSignal& as) -> void {
				err << "\"" << as.filename << "\"\n";
			};
			auto print_traditional_tuple = [&err](const TraditionalSignal& ts) -> void {
				err << "(\"" << ts.glow_filename << ".{x,csv,b3d}\", \"" << ts.glow_filename
				    << ".{bmp,png,jpg}\")";
			};

			err << "Signal(Traditional) is overwriting signal at index (" << inst.signal_index
			    << "). Old Value: ";
			old_value.match(print_animated_file, print_traditional_tuple);
			err << "New Value: ";
			print_traditional_tuple(this_signal);
			err << ".";
			add_error(errors_, getFilename(inst.file_index), inst.line, err);
		}
	}
} // namespace bve::parsers::csv_rw_route
