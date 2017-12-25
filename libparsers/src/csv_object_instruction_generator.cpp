#include "core/math.hpp"
#include "csv_object.hpp"
#include "csv_parser.hpp"
#include "utils.hpp"
#include <iostream>
#include <map>
#include <stdexcept>

using namespace std::string_literals;

namespace parsers {
namespace csv_object {
	namespace {
		instruction create_instruction_createmeshbuilder(const std::vector<csv::csv_token>& arguments) {
			(void) arguments;
			return instructions::CreateMeshBuilder{};
		}

		instruction create_instruction_addvertex(const std::vector<csv::csv_token>& arguments) {
			instructions::AddVertex av{};
			switch (arguments.size()) {
				default:
				case 7:
					av.nZ = util::parse_loose_float(arguments[6].text, 0);
					// fall through
				case 6:
					av.nY = util::parse_loose_float(arguments[5].text, 0);
					// fall through
				case 5:
					av.nX = util::parse_loose_float(arguments[4].text, 0);
					// fall through
				case 4:
					av.vZ = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					av.vY = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					av.vX = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}

			return av;
		}

		instruction _create_instruction_addface_impl(const std::vector<csv::csv_token>& arguments, bool two) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction addface");
			}
			instructions::AddFace af{};
			for (std::size_t i = 1; i < arguments.size(); ++i) {
				try {
					af.vertices.emplace_back(util::parse_loose_integer(arguments[i].text));
				}
				catch (std::invalid_argument& e) {
					if (af.vertices.size() < 3) {
						throw e;
					}
				}
			}
			af.two = two;
			return af;
		}

		instruction create_instruction_addface(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_addface_impl(arguments, false);
		}

		instruction create_instruction_addface2(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_addface_impl(arguments, true);
		}

		instruction create_instruction_cube(const std::vector<csv::csv_token>& arguments) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction cube");
			}
			instructions::Cube cu{};
			cu.HalfWidth = util::parse_loose_float(arguments[1].text);
			cu.HalfHeight = util::parse_loose_float(arguments[2].text);
			cu.HalfDepth = util::parse_loose_float(arguments[3].text);
			return cu;
		}

		instruction create_instruction_cylinder(const std::vector<csv::csv_token>& arguments) {
			if (arguments.size() < 5) {
				throw std::invalid_argument("Creation of instruction cylinder");
			}
			instructions::Cylinder cy{};
			cy.sides = std::size_t(util::parse_loose_integer(arguments[1].text));
			cy.UpperRadius = util::parse_loose_float(arguments[2].text);
			cy.LowerRadius = util::parse_loose_float(arguments[3].text);
			cy.Height = util::parse_loose_float(arguments[4].text);
			return cy;
		}

		instruction _create_instruction_translate_impl(const std::vector<csv::csv_token>& arguments, bool all) {
			instructions::Translate tl{};
			switch (arguments.size()) {
				default:
				case 4:
					tl.Z = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					tl.Y = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					tl.X = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}
			tl.all = all;
			return tl;
		}

		instruction create_instruction_translate(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_translate_impl(arguments, false);
		}
		instruction create_instruction_translateall(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_translate_impl(arguments, true);
		}

		instruction _create_instruction_scale_impl(const std::vector<csv::csv_token>& arguments, bool all) {
			instructions::Scale sc{};
			switch (arguments.size()) {
				default:
				case 4:
					sc.Z = util::parse_loose_float(arguments[3].text, 1);
					// fall through
				case 3:
					sc.Y = util::parse_loose_float(arguments[2].text, 1);
					// fall through
				case 2:
					sc.X = util::parse_loose_float(arguments[1].text, 1);
					// fall through
				case 1:
					break;
			}
			sc.all = all;
			return sc;
		}

		instruction create_instruction_scale(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_scale_impl(arguments, false);
		}
		instruction create_instruction_scaleall(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_scale_impl(arguments, true);
		}

		instruction _create_instruction_rotate_impl(const std::vector<csv::csv_token>& arguments, bool all) {
			instructions::Rotate ro{};
			switch (arguments.size()) {
				default:
				case 5:
					ro.Angle = util::parse_loose_float(arguments[4].text, 0);
					// fall through
				case 4:
					ro.Z = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					ro.Y = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					ro.X = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}
			ro.all = all;
			return ro;
		}

		instruction create_instruction_rotate(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_rotate_impl(arguments, false);
		}
		instruction create_instruction_rotateall(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_rotate_impl(arguments, true);
		}

		instruction _create_instruction_shear_impl(const std::vector<csv::csv_token>& arguments, bool all) {
			instructions::Shear sh{};
			switch (arguments.size()) {
				default:
				case 8:
					sh.r = util::parse_loose_float(arguments[7].text, 0);
					// fall through
				case 7:
					sh.sZ = util::parse_loose_float(arguments[6].text, 0);
					// fall through
				case 6:
					sh.sY = util::parse_loose_float(arguments[5].text, 0);
					// fall through
				case 5:
					sh.sX = util::parse_loose_float(arguments[4].text, 0);
					// fall through
				case 4:
					sh.dZ = util::parse_loose_float(arguments[3].text, 0);
					// fall through
				case 3:
					sh.dY = util::parse_loose_float(arguments[2].text, 0);
					// fall through
				case 2:
					sh.dX = util::parse_loose_float(arguments[1].text, 0);
					// fall through
				case 1:
					break;
			}
			sh.all = all;
			return sh;
		}

		instruction create_instruction_shear(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_shear_impl(arguments, false);
		}
		instruction create_instruction_shearall(const std::vector<csv::csv_token>& arguments) {
			return _create_instruction_shear_impl(arguments, true);
		}

		instruction create_instruction_setcolor(const std::vector<csv::csv_token>& arguments) {
			instructions::SetColor sc{};
			switch (arguments.size()) {
				default:
				case 5:
					sc.Alpha =
					    uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[4].text, 255), 0, 255));
					// fall through
				case 4:
					sc.Blue = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[3].text, 255), 0, 255));
					// fall through
				case 3:
					sc.Green =
					    uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 255), 0, 255));
					// fall through
				case 2:
					sc.Red = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[1].text, 255), 0, 255));
					// fall through
				case 1:
					break;
			}
			return sc;
		}

		instruction create_instruction_setemissivecolor(const std::vector<csv::csv_token>& arguments) {
			instructions::SetEmissiveColor sec{};
			switch (arguments.size()) {
				default:
				case 4:
					sec.Blue = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[3].text, 0), 0, 255));
					// fall through
				case 3:
					sec.Green = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 0), 0, 255));
					// fall through
				case 2:
					sec.Red = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[1].text, 0), 0, 255));
					// fall through
				case 1:
					break;
			}
			return sec;
		}

		instruction create_instruction_setblendmode(const std::vector<csv::csv_token>& arguments) {
			instructions::SetBlendMode sbm{};
			switch (arguments.size()) {
				default:
				case 4:
					if (util::match_text(arguments[3].text, "divideexponent4")) {
						sbm.GlowAttenuationMode = mesh_t::GlowAttenuationMode_t::DivideExponent4;
					}
					else if (util::match_text(arguments[3].text, "divideexponent2")) {
						sbm.GlowAttenuationMode = mesh_t::GlowAttenuationMode_t::DivideExponent2;
					}
					// fall through
				case 3:
					sbm.GlowHalfDistance =
					    uint16_t(openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 0), 0, 4095));
					// fall through
				case 2:
					if (util::match_text(arguments[1].text, "normal")) {
						sbm.BlendMode = mesh_t::BlendMode_t::Normal;
					}
					else if (util::match_text(arguments[1].text, "additive")) {
						sbm.BlendMode = mesh_t::BlendMode_t::Additive;
					}
					// fall through
				case 1:
					break;
			}
			return sbm;
		}

		instruction create_instruction_loadtexture(const std::vector<csv::csv_token>& arguments) {
			instructions::LoadTexture lt{};
			switch (arguments.size()) {
				default:
				case 3:
					lt.NighttimeTexture = arguments[2].text;
					// fall through
				case 2:
					lt.DaytimeTexture = arguments[1].text;
					// fall through
				case 1:
					break;
			}
			return lt;
		}

		instruction create_instruction_setdecaltransparentcolor(const std::vector<csv::csv_token>& arguments) {
			instructions::SetDecalTransparentColor sdtc{};
			switch (arguments.size()) {
				default:
				case 4:
					sdtc.Blue = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[3].text, 0), 0, 255));
					// fall through
				case 3:
					sdtc.Green =
					    uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[2].text, 0), 0, 255));
					// fall through
				case 2:
					sdtc.Red = uint8_t(openbve2::math::clamp(util::parse_loose_integer(arguments[1].text, 0), 0, 255));
					// fall through
				case 1:
					break;
			}
			return sdtc;
		}
		instruction create_instruction_settexturecoordinates(const std::vector<csv::csv_token>& arguments) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction settexturecoordinates");
			}
			instructions::SetTextureCoordinates stc{};
			stc.VertexIndex = std::size_t(util::parse_loose_integer(arguments[1].text));
			stc.X = util::parse_loose_float(arguments[2].text);
			stc.Y = util::parse_loose_float(arguments[3].text);
			return stc;
		}

		std::map<std::string, instruction (*)(const std::vector<csv::csv_token>&)> function_mapping{
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
		    {"color"s, &create_instruction_setcolor},
		    {"emissivecolor"s, &create_instruction_setemissivecolor},
		    {"blendmode"s, &create_instruction_setblendmode},
		    {"load"s, &create_instruction_loadtexture},
		    {"transparent"s, &create_instruction_setdecaltransparentcolor},
		    {"coordinates"s, &create_instruction_settexturecoordinates},
		};
	} // namespace

	instruction_list create_instructions(std::string text, file_type ft) {
		instruction_list il;

		util::remove_comments(text, ';');

		csv::parsed_csv csv = csv::parse(text, ft == file_type::b3d ? csv::split_first_column::yes : csv::split_first_column::no);

		for (auto& row : csv) {
			if (row.empty() || row[0].text.empty()) {
				continue;
			}

			std::cout << uint16_t(uint8_t(row[0].text[0])) << '\n';

			instruction ins;
			auto found_func = function_mapping.find(util::lower(row[0].text));
			if (found_func == function_mapping.end()) {
				ins = instructions::Error{"Function \""s + row[0].text + "\" not found"s};
			}
			else {
				try {
					ins = (found_func->second)(row);
				}
				catch (const std::invalid_argument& e) {
					ins = instructions::Error{e.what()};
				}
			}

			// Set line number for appropriate debugging help
			boost::apply_visitor([&row](auto& x) { x.line = row[0].line_begin; }, ins);

			il.emplace_back(std::move(ins));
		}

		return il;
	} // namespace csv_object
} // namespace csv_object
} // namespace parsers
