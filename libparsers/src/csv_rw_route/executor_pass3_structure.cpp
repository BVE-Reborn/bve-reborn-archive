#include "executor_pass3.hpp"
#include <sstream>

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::structure::Command& instr) {
		auto add_and_warn = [&](auto& container, const char* command_name) {
			auto insert_pair =
			    container.insert(std::pair<std::size_t, std::string>{instr.structure_index, instr.filename});

			auto& iterator = insert_pair.first;
			auto& inserted = insert_pair.second;

			if (!inserted) {
				auto previous_filename = iterator->second;
				iterator->second = instr.filename;
				std::ostringstream err;
				err << command_name << " overwriting index number " << instr.structure_index << ". Old Filename: \""
				    << previous_filename << "\". Current Filename: \"" << instr.filename << "\".";
				_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
			}
		};

		switch (instr.command) {
			case instructions::structure::Command::Ground:
				add_and_warn(object_ground_mapping, "Structure.Ground");
				break;
			case instructions::structure::Command::Rail:
				add_and_warn(object_rail_mapping, "Structure.Rail");
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
	void pass3_executor::operator()(const instructions::structure::Pole& instr) {
		std::pair<std::pair<std::size_t, std::size_t>, std::string> value{
		    {instr.additional_rails, instr.pole_structure_index}, instr.filename};
		auto insert_pair = object_pole_mapping.insert(value);

		auto& iterator = insert_pair.first;
		auto& inserted = insert_pair.second;

		if (!inserted) {
			auto previous_filename = iterator->second;
			iterator->second = instr.filename;
			std::ostringstream err;
			err << "Structure.Pole overwriting pair (" << instr.additional_rails << ", " << instr.pole_structure_index
			    << "). Old Pair: \"" << previous_filename << "\". Current Filename: \"" << instr.filename << "\".";
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>({instr.line, err.str()});
		}
	}
} // namespace csv_rw_route
} // namespace parsers
