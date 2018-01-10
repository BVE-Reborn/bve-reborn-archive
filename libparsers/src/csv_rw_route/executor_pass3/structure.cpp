#include "../executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::structure::Command& inst) {
		auto add_and_warn = [&](auto& container, const char* command_name) {
			auto filename_iter = add_object_filename(inst.filename);

			auto insert_pair = container.insert(std::make_pair(inst.structure_index, filename_iter));

			auto& iterator = insert_pair.first;
			auto& inserted = insert_pair.second;

			if (!inserted) {
				auto& previous_filename = *iterator->second;
				iterator->second = filename_iter;
				std::ostringstream err;
				err << command_name << " overwriting index #" << inst.structure_index << ". Old Filename: \""
				    << previous_filename << "\". Current Filename: \"" << *filename_iter << "\".";
				_errors[get_filename(inst.file_index)].emplace_back<errors::error_t>({inst.line, err.str()});
			}
		};

		auto add_and_warn_cycle = [&](auto& container, const char* command_name) {
			auto filename_iter = add_object_filename(inst.filename);

			auto insert_pair = container.insert(std::make_pair(inst.structure_index, filename_iter));

			auto& iterator = insert_pair.first;
			auto& inserted = insert_pair.second;

			if (!inserted) {
				auto old_value = iterator->second;
				iterator->second = filename_iter;

				std::ostringstream err;

				err << command_name << " overwriting index number " << inst.structure_index << ". Old Filename: \"";
				print_cycle_type(err, old_value);
				err << "\". Current Filename: \"";
				print_cycle_type(err, filename_iter);
				err << "\".";

				_errors[get_filename(inst.file_index)].emplace_back<errors::error_t>({inst.line, err.str()});
			}
		};

		switch (inst.command) {
			case instructions::structure::Command::Ground:
				add_and_warn_cycle(object_ground_mapping, "Structure.Ground");
				break;
			case instructions::structure::Command::Rail:
				add_and_warn_cycle(object_rail_mapping, "Structure.Rail");
				break;
			case instructions::structure::Command::WallL:
				add_and_warn(object_wallL_mapping, "Structure.WallL");
				break;
			case instructions::structure::Command::WallR:
				add_and_warn(object_wallR_mapping, "Structure.WallR");
				break;
			case instructions::structure::Command::DikeL:
				add_and_warn(object_dikeL_mapping, "Structure.DikeL");
				break;
			case instructions::structure::Command::DikeR:
				add_and_warn(object_dikeR_mapping, "Structure.DikeR");
				break;
			case instructions::structure::Command::FormL:
				add_and_warn(object_formL_mapping, "Structure.FormL");
				break;
			case instructions::structure::Command::FormR:
				add_and_warn(object_formR_mapping, "Structure.FormR");
				break;
			case instructions::structure::Command::FormCL:
				add_and_warn(object_formCL_mapping, "Structure.FormCL");
				break;
			case instructions::structure::Command::FormCR:
				add_and_warn(object_formCR_mapping, "Structure.FormCR");
				break;
			case instructions::structure::Command::RoofL:
				add_and_warn(object_roofL_mapping, "Structure.RoofL");
				break;
			case instructions::structure::Command::RoofR:
				add_and_warn(object_roofR_mapping, "Structure.RoofR");
				break;
			case instructions::structure::Command::RoofCL:
				add_and_warn(object_roofCL_mapping, "Structure.RoofCL");
				break;
			case instructions::structure::Command::RoofCR:
				add_and_warn(object_roofCR_mapping, "Structure.RoofCR");
				break;
			case instructions::structure::Command::CrackL:
				add_and_warn(object_crackL_mapping, "Structure.CrackL");
				break;
			case instructions::structure::Command::CrackR:
				add_and_warn(object_crackR_mapping, "Structure.CrackR");
				break;
			case instructions::structure::Command::FreeObj:
				add_and_warn(object_freeobj_mapping, "Structure.Freeobj");
				break;
			case instructions::structure::Command::Beacon:
				add_and_warn(object_beacon_mapping, "Structure.Beacon");
				break;
			default:
				break;
		}
	}

	void pass3_executor::operator()(const instructions::structure::Pole& inst) {
		auto filename_iter = add_object_filename(inst.filename);

		auto value = std::make_pair(std::make_pair(inst.additional_rails, inst.pole_structure_index), filename_iter);
		auto insert_pair = object_pole_mapping.insert(value);

		auto& iterator = insert_pair.first;
		auto& inserted = insert_pair.second;

		if (!inserted) {
			auto& previous_filename = *iterator->second;
			iterator->second = filename_iter;
			std::ostringstream err;
			err << "Structure.Pole overwriting pair (" << inst.additional_rails << ", " << inst.pole_structure_index
			    << "). Old Pair: \"" << previous_filename << "\". Current Filename: \"" << *filename_iter << "\".";
			_errors[get_filename(inst.file_index)].emplace_back<errors::error_t>({inst.line, err.str()});
		}
	}
} // namespace csv_rw_route
} // namespace parsers
