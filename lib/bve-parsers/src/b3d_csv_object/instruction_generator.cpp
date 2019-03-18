#include "b3d_csv_object.hpp"
#include "parsers/csv.hpp"
#include "util/math.hpp"
#include "util/parsing.hpp"
#include <gsl/gsl_util>
#include <map>
#include <stdexcept>

using namespace std::string_literals;

// ReSharper disable once CppInconsistentNaming
namespace bve::parsers::b3d_csv_object {
	namespace {
		Instruction create_instruction_createmeshbuilder(const std::vector<csv::CSVToken>& arguments) {
			(void) arguments;
			return instructions::CreateMeshBuilder{};
		}

		Instruction create_instruction_addvertex(const std::vector<csv::CSVToken>& arguments) {
			instructions::AddVertex av{};
			switch (arguments.size()) {
				default:
				case 7:
					av.n_z = util::parsers::parse_loose_float(arguments[6].text, 0);
					[[fallthrough]];
				case 6:
					av.n_y = util::parsers::parse_loose_float(arguments[5].text, 0);
					[[fallthrough]];
				case 5:
					av.n_x = util::parsers::parse_loose_float(arguments[4].text, 0);
					[[fallthrough]];
				case 4:
					av.v_z = util::parsers::parse_loose_float(arguments[3].text, 0);
					[[fallthrough]];
				case 3:
					av.v_y = util::parsers::parse_loose_float(arguments[2].text, 0);
					[[fallthrough]];
				case 2:
					av.v_x = util::parsers::parse_loose_float(arguments[1].text, 0);
					[[fallthrough]];
				case 1:
					break;
			}

			return av;
		}

		Instruction create_instruction_addface_impl(const std::vector<csv::CSVToken>& arguments, instructions::Sides const sides) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction addface");
			}
			instructions::AddFace af{};
			for (std::size_t i = 1; i < arguments.size(); ++i) {
				try {
					af.vertices.emplace_back(gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(arguments[i].text)));
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

		Instruction create_instruction_addface(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_addface_impl(arguments, instructions::Sides::one);
		}

		Instruction create_instruction_addface2(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_addface_impl(arguments, instructions::Sides::two);
		}

		Instruction create_instruction_cube(const std::vector<csv::CSVToken>& arguments) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction cube");
			}
			instructions::Cube cu{};
			cu.half_width = util::parsers::parse_loose_float(arguments[1].text);
			cu.half_height = util::parsers::parse_loose_float(arguments[2].text);
			cu.half_depth = util::parsers::parse_loose_float(arguments[3].text);
			return cu;
		}

		Instruction create_instruction_cylinder(const std::vector<csv::CSVToken>& arguments) {
			if (arguments.size() < 5) {
				throw std::invalid_argument("Creation of instruction cylinder");
			}
			instructions::Cylinder cy{};
			cy.sides = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(arguments[1].text));
			cy.upper_radius = util::parsers::parse_loose_float(arguments[2].text);
			cy.lower_radius = util::parsers::parse_loose_float(arguments[3].text);
			cy.height = util::parsers::parse_loose_float(arguments[4].text);
			return cy;
		}

		Instruction create_instruction_translate_impl(const std::vector<csv::CSVToken>& arguments, instructions::ApplyTo const applies_to) {
			instructions::Translate tl{};
			switch (arguments.size()) {
				default:
				case 4:
					tl.z = util::parsers::parse_loose_float(arguments[3].text, 0);
					[[fallthrough]];
				case 3:
					tl.y = util::parsers::parse_loose_float(arguments[2].text, 0);
					[[fallthrough]];
				case 2:
					tl.x = util::parsers::parse_loose_float(arguments[1].text, 0);
					[[fallthrough]];
				case 1:
					break;
			}
			tl.applies_to = applies_to;
			return tl;
		}

		Instruction create_instruction_translate(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_translate_impl(arguments, instructions::ApplyTo::single_mesh);
		}
		Instruction create_instruction_translateall(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_translate_impl(arguments, instructions::ApplyTo::all_meshes);
		}

		Instruction create_instruction_scale_impl(const std::vector<csv::CSVToken>& arguments, instructions::ApplyTo const applies_to) {
			instructions::Scale sc{};
			switch (arguments.size()) {
				default:
				case 4:
					sc.z = util::parsers::parse_loose_float(arguments[3].text, 1);
					[[fallthrough]];
				case 3:
					sc.y = util::parsers::parse_loose_float(arguments[2].text, 1);
					[[fallthrough]];
				case 2:
					sc.x = util::parsers::parse_loose_float(arguments[1].text, 1);
					[[fallthrough]];
				case 1:
					break;
			}
			sc.applies_to = applies_to;
			return sc;
		}

		Instruction create_instruction_scale(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_scale_impl(arguments, instructions::ApplyTo::single_mesh);
		}
		Instruction create_instruction_scaleall(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_scale_impl(arguments, instructions::ApplyTo::all_meshes);
		}

		Instruction create_instruction_rotate_impl(const std::vector<csv::CSVToken>& arguments, instructions::ApplyTo const applies_to) {
			instructions::Rotate ro{};
			switch (arguments.size()) {
				default:
				case 5:
					ro.angle = util::parsers::parse_loose_float(arguments[4].text, 0);
					[[fallthrough]];
				case 4:
					ro.z = util::parsers::parse_loose_float(arguments[3].text, 0);
					[[fallthrough]];
				case 3:
					ro.y = util::parsers::parse_loose_float(arguments[2].text, 0);
					[[fallthrough]];
				case 2:
					ro.x = util::parsers::parse_loose_float(arguments[1].text, 0);
					[[fallthrough]];
				case 1:
					break;
			}
			ro.applies_to = applies_to;
			return ro;
		}

		Instruction create_instruction_rotate(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_rotate_impl(arguments, instructions::ApplyTo::single_mesh);
		}
		Instruction create_instruction_rotateall(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_rotate_impl(arguments, instructions::ApplyTo::all_meshes);
		}

		Instruction create_instruction_shear_impl(const std::vector<csv::CSVToken>& arguments, instructions::ApplyTo const applies_to) {
			instructions::Shear sh{};
			switch (arguments.size()) {
				default:
				case 8:
					sh.r = util::parsers::parse_loose_float(arguments[7].text, 0);
					[[fallthrough]];
				case 7:
					sh.s_z = util::parsers::parse_loose_float(arguments[6].text, 0);
					[[fallthrough]];
				case 6:
					sh.s_y = util::parsers::parse_loose_float(arguments[5].text, 0);
					[[fallthrough]];
				case 5:
					sh.s_x = util::parsers::parse_loose_float(arguments[4].text, 0);
					[[fallthrough]];
				case 4:
					sh.d_z = util::parsers::parse_loose_float(arguments[3].text, 0);
					[[fallthrough]];
				case 3:
					sh.d_y = util::parsers::parse_loose_float(arguments[2].text, 0);
					[[fallthrough]];
				case 2:
					sh.d_x = util::parsers::parse_loose_float(arguments[1].text, 0);
					[[fallthrough]];
				case 1:
					break;
			}
			sh.applies_to = applies_to;
			return sh;
		}

		Instruction create_instruction_shear(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_shear_impl(arguments, instructions::ApplyTo::single_mesh);
		}
		Instruction create_instruction_shearall(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_shear_impl(arguments, instructions::ApplyTo::all_meshes);
		}

		Instruction create_instruction_mirror_impl(const std::vector<csv::CSVToken>& arguments, instructions::ApplyTo const applies_to) {
			instructions::Mirror mirror{};
			switch (arguments.size()) {
				default:
				case 4:
					mirror.z = util::parsers::parse_loose_integer(arguments[3].text, 0) != 0;
					[[fallthrough]];
				case 3:
					mirror.y = util::parsers::parse_loose_integer(arguments[2].text, 0) != 0;
					[[fallthrough]];
				case 2:
					mirror.x = util::parsers::parse_loose_integer(arguments[1].text, 0) != 0;
					[[fallthrough]];
				case 1:
					break;
			}
			mirror.applies_to = applies_to;
			return mirror;
		}

		Instruction create_instruction_mirror(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_mirror_impl(arguments, instructions::ApplyTo::single_mesh);
		}
		Instruction create_instruction_mirrorall(const std::vector<csv::CSVToken>& arguments) {
			return create_instruction_mirror_impl(arguments, instructions::ApplyTo::all_meshes);
		}

		Instruction create_instruction_setcolor(const std::vector<csv::CSVToken>& arguments) {
			instructions::SetColor sc{};
			switch (arguments.size()) {
				default:
				case 5:
					sc.alpha = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[4].text, 255), 0, 255));
					[[fallthrough]];
				case 4:
					sc.blue = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[3].text, 255), 0, 255));
					[[fallthrough]];
				case 3:
					sc.green = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[2].text, 255), 0, 255));
					[[fallthrough]];
				case 2:
					sc.red = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[1].text, 255), 0, 255));
					[[fallthrough]];
				case 1:
					break;
			}
			return sc;
		}

		Instruction create_instruction_setemissivecolor(const std::vector<csv::CSVToken>& arguments) {
			instructions::SetEmissiveColor sec{};
			switch (arguments.size()) {
				default:
				case 4:
					sec.blue = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[3].text, 0), 0, 255));
					[[fallthrough]];
				case 3:
					sec.green = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[2].text, 0), 0, 255));
					[[fallthrough]];
				case 2:
					sec.red = uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[1].text, 0), 0, 255));
					[[fallthrough]];
				case 1:
					break;
			}
			return sec;
		}

		Instruction create_instruction_setblendmode(const std::vector<csv::CSVToken>& arguments) {
			instructions::SetBlendMode sbm{};
			switch (arguments.size()) {
				default:
				case 4:
					if (util::parsers::match_against_lower(arguments[3].text, "divideexponent4")) {
						sbm.glow_attenuation_mode = Mesh::GlowAttenuationMode::divide_exponent4;
					}
					else if (util::parsers::match_against_lower(arguments[3].text, "divideexponent2")) {
						sbm.glow_attenuation_mode = Mesh::GlowAttenuationMode::divide_exponent2;
					}
					[[fallthrough]];
				case 3:
					sbm.glow_half_distance = uint16_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[2].text, 0), 0, 4095));
					[[fallthrough]];
				case 2:
					if (util::parsers::match_against_lower(arguments[1].text, "normal")) {
						sbm.blend_mode = Mesh::BlendMode::normal;
					}
					else if (util::parsers::match_against_lower(arguments[1].text, "additive")) {
						sbm.blend_mode = Mesh::BlendMode::additive;
					}
					[[fallthrough]];
				case 1:
					break;
			}
			return sbm;
		}

		Instruction create_instruction_loadtexture(const std::vector<csv::CSVToken>& arguments) {
			instructions::LoadTexture lt{};
			switch (arguments.size()) {
				default:
				case 3:
					lt.nighttime_texture = arguments[2].text;
					[[fallthrough]];
				case 2:
					lt.daytime_texture = arguments[1].text;
					[[fallthrough]];
				case 1:
					break;
			}
			return lt;
		}

		Instruction create_instruction_setdecaltransparentcolor(const std::vector<csv::CSVToken>& arguments) {
			instructions::SetDecalTransparentColor decal_transparent_color{};
			switch (arguments.size()) {
				default:
				case 4:
					decal_transparent_color.blue =
					    uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[3].text, 0), 0, 255));
					[[fallthrough]];
				case 3:
					decal_transparent_color.green =
					    uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[2].text, 0), 0, 255));
					[[fallthrough]];
				case 2:
					decal_transparent_color.red =
					    uint8_t(util::math::clamp(util::parsers::parse_loose_integer(arguments[1].text, 0), 0, 255));
					[[fallthrough]];
				case 1:
					break;
			}
			return decal_transparent_color;
		}
		Instruction create_instruction_settexturecoordinates(const std::vector<csv::CSVToken>& arguments) {
			if (arguments.size() < 4) {
				throw std::invalid_argument("Creation of instruction settexturecoordinates");
			}
			instructions::SetTextureCoordinates stc{};
			stc.vertex_index = gsl::narrow<std::size_t>(util::parsers::parse_loose_integer(arguments[1].text));
			stc.x = util::parsers::parse_loose_float(arguments[2].text);
			stc.y = util::parsers::parse_loose_float(arguments[3].text);
			return stc;
		}

		const std::map<std::string, Instruction (*)(const std::vector<csv::CSVToken>&)> function_mapping{
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

	InstructionList create_instructions(std::string text, FileType const ft) {
		InstructionList il;

		util::parsers::remove_comments(text, ';');

		auto csv = parse(text, ft == FileType::b3d ? csv::SplitFirstColumn::yes : csv::SplitFirstColumn::no);

		for (auto& row : csv) {
			if (row.empty() || row[0].text.empty()) {
				continue;
			}
			if (ft == FileType::b3d && util::parsers::match_against_lower(row[0].text, "[texture]")) {
				continue;
			}
			if (ft == FileType::csv && util::parsers::match_against_lower(row[0].text, "generatenormals")) {
				continue;
			}

			Instruction ins;
			util::parsers::lower(row[0].text);
			auto const found_func = function_mapping.find(row[0].text);
			if (found_func == function_mapping.end()) {
				ins = instructions::Error{"Function \""s + row[0].text + "\" not found"s};
			}
			else {
				try {
					ins = found_func->second(row);
				}
				catch (const std::invalid_argument& e) {
					ins = instructions::Error{e.what()};
				}
			}

			// Set line number for appropriate debugging help
			apply_visitor([&row](auto& x) { x.line = row[0].line_begin; }, ins);

			il.emplace_back(std::move(ins));
		}

		return il;
	} // namespace b3d_csv_object
} // namespace bve::parsers::b3d_csv_object
