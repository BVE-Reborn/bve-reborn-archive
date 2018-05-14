#include "b3d_csv_object.hpp"
#include "core/math.hpp"
#include "csv.hpp"
#include "utils.hpp"
#include <gsl/gsl_util>
#include <map>
#include <stdexcept>

using namespace std::string_literals;

namespace parsers {
// ReSharper disable once CppInconsistentNaming
namespace b3d_csv_object {
	namespace {
		instruction create_instruction_createmeshbuilder(
		    const std::vector<csv::csv_token>& arguments) {
			(void) arguments;
			return instructions::create_mesh_builder{};
		}

		instruction create_instruction_addvertex(const std::vector<csv::csv_token>& arguments) {
			instructions::add_vertex av{};
			switch (arguments.size()) {
				default:
				case 7:
					av.n_z = util::parse_loose_float(arguments[6].text, 0);
					// fall through
				case 6:
					av.n_y = util::parse_loose_float(arguments[5].text, 0);
					// fall through
				case 5:
					av.n_x = util::parse_loose_float(arguments[4].text, 0);
					// fall through
				case 4:
					av.v_z = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					av.v_y = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					av.v_x = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}

			return av;
		}

		instruction create_instruction_addface_impl(const std::vector<csv::csv_token>& arguments,
		                                            instructions::sides const sides) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction addface");
			}
			instructions::add_face af{};
			for (std::size_t i = 1; i < arguments.size(); ++i) {
				try {
					af.vertices.emplace_back(
					    gsl::narrow<std::size_t>(util::parse_loose_integer(arguments[i].text)));
				}
				catch (std::invalid_argument&) {
					if (af.vertices.size() < 3) {
						throw;
					}
				}
			}
			af.side_count = sides;
			return af;
		}

		instruction create_instruction_addface(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_addface_impl(arguments, instructions::sides::one);
		}

		instruction create_instruction_addface2(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_addface_impl(arguments, instructions::sides::two);
		}

		instruction create_instruction_cube(const std::vector<csv::csv_token>& arguments) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction cube");
			}
			instructions::cube cu{};
			cu.half_width = util::parse_loose_float(arguments[1].text);
			cu.half_height = util::parse_loose_float(arguments[2].text);
			cu.half_depth = util::parse_loose_float(arguments[3].text);
			return cu;
		}

		instruction create_instruction_cylinder(const std::vector<csv::csv_token>& arguments) {
			if (arguments.size() < 5) {
				throw std::invalid_argument("Creation of instruction cylinder");
			}
			instructions::cylinder cy{};
			cy.sides = gsl::narrow<std::size_t>(util::parse_loose_integer(arguments[1].text));
			cy.upper_radius = util::parse_loose_float(arguments[2].text);
			cy.lower_radius = util::parse_loose_float(arguments[3].text);
			cy.height = util::parse_loose_float(arguments[4].text);
			return cy;
		}

		instruction create_instruction_translate_impl(const std::vector<csv::csv_token>& arguments,
		                                              instructions::apply_to const applies_to) {
			instructions::translate tl{};
			switch (arguments.size()) {
				default:
				case 4:
					tl.z = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					tl.y = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					tl.x = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}
			tl.applies_to = applies_to;
			return tl;
		}

		instruction create_instruction_translate(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_translate_impl(arguments,
			                                         instructions::apply_to::single_mesh);
		}
		instruction create_instruction_translateall(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_translate_impl(arguments, instructions::apply_to::all_meshes);
		}

		instruction create_instruction_scale_impl(const std::vector<csv::csv_token>& arguments,
		                                          instructions::apply_to const applies_to) {
			instructions::scale sc{};
			switch (arguments.size()) {
				default:
				case 4:
					sc.z = util::parse_loose_float(arguments[3].text, 1);
					// fall through
				case 3:
					sc.y = util::parse_loose_float(arguments[2].text, 1);
					// fall through
				case 2:
					sc.x = util::parse_loose_float(arguments[1].text, 1);
					// fall through
				case 1:
					break;
			}
			sc.applies_to = applies_to;
			return sc;
		}

		instruction create_instruction_scale(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_scale_impl(arguments, instructions::apply_to::single_mesh);
		}
		instruction create_instruction_scaleall(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_scale_impl(arguments, instructions::apply_to::all_meshes);
		}

		instruction create_instruction_rotate_impl(const std::vector<csv::csv_token>& arguments,
		                                           instructions::apply_to const applies_to) {
			instructions::rotate ro{};
			switch (arguments.size()) {
				default:
				case 5:
					ro.angle = util::parse_loose_float(arguments[4].text, 0);
					// fall through
				case 4:
					ro.z = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					ro.y = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					ro.x = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}
			ro.applies_to = applies_to;
			return ro;
		}

		instruction create_instruction_rotate(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_rotate_impl(arguments, instructions::apply_to::single_mesh);
		}
		instruction create_instruction_rotateall(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_rotate_impl(arguments, instructions::apply_to::all_meshes);
		}

		instruction create_instruction_shear_impl(const std::vector<csv::csv_token>& arguments,
		                                          instructions::apply_to const applies_to) {
			instructions::shear sh{};
			switch (arguments.size()) {
				default:
				case 8:
					sh.r = util::parse_loose_float(arguments[7].text, 0);
					// fall through
				case 7:
					sh.s_z = util::parse_loose_float(arguments[6].text, 0);
					// fall through
				case 6:
					sh.s_y = util::parse_loose_float(arguments[5].text, 0);
					// fall through
				case 5:
					sh.s_x = util::parse_loose_float(arguments[4].text, 0);
					// fall through
				case 4:
					sh.d_z = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					sh.d_y = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					sh.d_x = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}
			sh.applies_to = applies_to;
			return sh;
		}

		instruction create_instruction_shear(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_shear_impl(arguments, instructions::apply_to::single_mesh);
		}
		instruction create_instruction_shearall(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_shear_impl(arguments, instructions::apply_to::all_meshes);
		}

		instruction create_instruction_mirror_impl(const std::vector<csv::csv_token>& arguments,
		                                           instructions::apply_to const applies_to) {
			instructions::mirror mirror{};
			switch (arguments.size()) {
				default:
				case 4:
					mirror.z = util::parse_loose_integer(arguments[3].text, 0) != 0;
					// fall through
				case 3:
					mirror.y = util::parse_loose_integer(arguments[2].text, 0) != 0;
					// fall through
				case 2:
					mirror.x = util::parse_loose_integer(arguments[1].text, 0) != 0;
					// fall through
				case 1:
					break;
			}
			mirror.applies_to = applies_to;
			return mirror;
		}

		instruction create_instruction_mirror(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_mirror_impl(arguments, instructions::apply_to::single_mesh);
		}
		instruction create_instruction_mirrorall(const std::vector<csv::csv_token>& arguments) {
			return create_instruction_mirror_impl(arguments, instructions::apply_to::all_meshes);
		}

		instruction create_instruction_setcolor(const std::vector<csv::csv_token>& arguments) {
			instructions::set_color sc{};
			switch (arguments.size()) {
				default:
				case 5:
					sc.alpha = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[4].text, 255), 0,
					                          255));
					// fall through
				case 4:
					sc.blue = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[3].text, 255), 0,
					                          255));
					// fall through
				case 3:
					sc.green = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 255), 0,
					                          255));
					// fall through
				case 2:
					sc.red = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[1].text, 255), 0,
					                          255));
					// fall through
				case 1:
					break;
			}
			return sc;
		}

		instruction create_instruction_setemissivecolor(
		    const std::vector<csv::csv_token>& arguments) {
			instructions::set_emissive_color sec{};
			switch (arguments.size()) {
				default:
				case 4:
					sec.blue = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[3].text, 0), 0,
					                          255));
					// fall through
				case 3:
					sec.green = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 0), 0,
					                          255));
					// fall through
				case 2:
					sec.red = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[1].text, 0), 0,
					                          255));
					// fall through
				case 1:
					break;
			}
			return sec;
		}

		instruction create_instruction_setblendmode(const std::vector<csv::csv_token>& arguments) {
			instructions::set_blend_mode sbm{};
			switch (arguments.size()) {
				default:
				case 4:
					if (util::match_against_lower(arguments[3].text, "divideexponent4")) {
						sbm.glow_attenuation_mode =
						    mesh_t::glow_attenuation_mode_t::divide_exponent4;
					}
					else if (util::match_against_lower(arguments[3].text, "divideexponent2")) {
						sbm.glow_attenuation_mode =
						    mesh_t::glow_attenuation_mode_t::divide_exponent2;
					}
					// fall through
				case 3:
					sbm.glow_half_distance = uint16_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 0), 0,
					                          4095));
					// fall through
				case 2:
					if (util::match_against_lower(arguments[1].text, "normal")) {
						sbm.blend_mode = mesh_t::blend_mode_t::normal;
					}
					else if (util::match_against_lower(arguments[1].text, "additive")) {
						sbm.blend_mode = mesh_t::blend_mode_t::additive;
					}
					// fall through
				case 1:
					break;
			}
			return sbm;
		}

		instruction create_instruction_loadtexture(const std::vector<csv::csv_token>& arguments) {
			instructions::load_texture lt{};
			switch (arguments.size()) {
				default:
				case 3:
					lt.nighttime_texture = arguments[2].text;
					// fall through
				case 2:
					lt.daytime_texture = arguments[1].text;
					// fall through
				case 1:
					break;
			}
			return lt;
		}

		instruction create_instruction_setdecaltransparentcolor(
		    const std::vector<csv::csv_token>& arguments) {
			instructions::set_decal_transparent_color sdtc{};
			switch (arguments.size()) {
				default:
				case 4:
					sdtc.blue = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[3].text, 0), 0,
					                          255));
					// fall through
				case 3:
					sdtc.green = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 0), 0,
					                          255));
					// fall through
				case 2:
					sdtc.red = uint8_t(
					    openbve2::math::clamp(util::parse_loose_integer(arguments[1].text, 0), 0,
					                          255));
					// fall through
				case 1:
					break;
			}
			return sdtc;
		}
		instruction create_instruction_settexturecoordinates(
		    const std::vector<csv::csv_token>& arguments) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction settexturecoordinates");
			}
			instructions::set_texture_coordinates stc{};
			stc.vertex_index =
			    gsl::narrow<std::size_t>(util::parse_loose_integer(arguments[1].text));
			stc.x = util::parse_loose_float(arguments[2].text);
			stc.y = util::parse_loose_float(arguments[3].text);
			return stc;
		}

		const std::map<std::string, instruction (*)(const std::vector<csv::csv_token>&)>
		    function_mapping{
		        // .csv
		        {"createmeshbuilder"s, &create_instruction_createmeshbuilder},
		        {"addvertex"s, &create_instruction_addvertex},
		        {"addface"s, &create_instruction_addface},
		        {"addface2"s, &create_instruction_addface2},
		        {"cube"s, &create_instruction_cube},
		        {"cylinder"s, &create_instruction_cylinder},
		        {"translate"s, &create_instruction_translate},
		        {"translateall"s, &create_instruction_translateall},
		        {"scale"s, &create_instruction_scale},
		        {"scaleall"s, &create_instruction_scaleall},
		        {"rotate"s, &create_instruction_rotate},
		        {"rotateall"s, &create_instruction_rotateall},
		        {"shear"s, &create_instruction_shear},
		        {"shearall"s, &create_instruction_shearall},
		        {"mirror"s, &create_instruction_mirror},
		        {"mirrorall"s, &create_instruction_mirrorall},
		        {"setcolor"s, &create_instruction_setcolor},
		        {"setemissivecolor"s, &create_instruction_setemissivecolor},
		        {"setblendmode"s, &create_instruction_setblendmode},
		        {"loadtexture"s, &create_instruction_loadtexture},
		        {"setdecaltransparentcolor"s, &create_instruction_setdecaltransparentcolor},
		        {"settexturecoordinates"s, &create_instruction_settexturecoordinates},

		        // .b3d
		        {"[meshbuilder]"s, &create_instruction_createmeshbuilder},
		        {"vertex"s, &create_instruction_addvertex},
		        {"face"s, &create_instruction_addface},
		        {"face2"s, &create_instruction_addface2},
		        {"cube"s, &create_instruction_cube},
		        {"cylinder"s, &create_instruction_cylinder},
		        {"translate"s, &create_instruction_translate},
		        {"translateall"s, &create_instruction_translateall},
		        {"scale"s, &create_instruction_scale},
		        {"scaleall"s, &create_instruction_scaleall},
		        {"rotate"s, &create_instruction_rotate},
		        {"rotateall"s, &create_instruction_rotateall},
		        {"shear"s, &create_instruction_shear},
		        {"shearall"s, &create_instruction_shearall},
		        {"mirror"s, &create_instruction_mirror},
		        {"mirrorall"s, &create_instruction_mirrorall},
		        {"color"s, &create_instruction_setcolor},
		        {"emissivecolor"s, &create_instruction_setemissivecolor},
		        {"blendmode"s, &create_instruction_setblendmode},
		        {"load"s, &create_instruction_loadtexture},
		        {"transparent"s, &create_instruction_setdecaltransparentcolor},
		        {"coordinates"s, &create_instruction_settexturecoordinates},
		    };
	} // namespace

	instruction_list create_instructions(std::string text, file_type const ft) {
		instruction_list il;

		util::remove_comments(text, ';');

		auto csv = parse(text, ft == file_type::b3d ? csv::split_first_column::yes
		                                            : csv::split_first_column::no);

		for (auto& row : csv) {
			if (row.empty() || row[0].text.empty()) {
				continue;
			}
			if (ft == file_type::b3d && util::match_against_lower(row[0].text, "[texture]")) {
				continue;
			}
			if (ft == file_type::csv && util::match_against_lower(row[0].text, "generatenormals")) {
				continue;
			}

			instruction ins;
			util::lower(row[0].text);
			auto const found_func = function_mapping.find(row[0].text);
			if (found_func == function_mapping.end()) {
				ins = instructions::error{"Function \""s + row[0].text + "\" not found"s};
			}
			else {
				try {
					ins = found_func->second(row);
				}
				catch (const std::invalid_argument& e) {
					ins = instructions::error{e.what()};
				}
			}

			// Set line number for appropriate debugging help
			apply_visitor([&row](auto& x) { x.line = row[0].line_begin; }, ins);

			il.emplace_back(std::move(ins));
		}

		return il;
	} // namespace b3d_csv_object
} // namespace b3d_csv_object
} // namespace parsers
