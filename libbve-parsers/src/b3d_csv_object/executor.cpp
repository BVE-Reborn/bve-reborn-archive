#include "b3d_csv_object.hpp"
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <sstream>

// ReSharper disable once CppInconsistentNaming
#define _USE_MATH_DEFINES
#include <math.h> // NOLINT

namespace parsers::b3d_csv_object {
	namespace {
		// Following
		// https://stackoverflow.com/questions/3574680/sort-based-on-multiple-things-in-c
		// Sort based on properties to get all faces with same properties next
		// to each other
		bool sort_function(
		    const instructions::parsed_csv_object_builder::untriangulated_face_t& face1,
		    const instructions::parsed_csv_object_builder::untriangulated_face_t& face2) {
			auto& data1 = face1.data;
			auto& data2 = face2.data;

			if (data1.texture < data2.texture) {
				return true;
			}
			if (data2.texture > data1.texture) {
				return false;
			}

			// Need to create an order, though the order doesn't matter as long
			// as they are not equal
			auto const dtc_equal = data1.decal_transparent_color == data2.decal_transparent_color;
			auto const dtca = compAdd(data1.decal_transparent_color);
			auto const dtcb = compAdd(data2.decal_transparent_color);
			if (!dtc_equal && dtca < dtcb) {
				return true;
			}
			if (!dtc_equal && dtca >= dtcb) {
				return false;
			}

			if (static_cast<int>(data1.has_decal_transparent_color)
			    < static_cast<int>(data2.has_decal_transparent_color)) {
				return true;
			}
			if (static_cast<int>(data2.has_decal_transparent_color)
			    > static_cast<int>(data1.has_decal_transparent_color)) {
				return false;
			}

			// blend mode integer
			auto const bmia =
			    std::underlying_type<decltype(data1.blend_mode)>::type(data1.blend_mode);
			auto const bmib =
			    std::underlying_type<decltype(data2.blend_mode)>::type(data2.blend_mode);
			if (bmia < bmib) {
				return true;
			}
			if (bmib > bmia) {
				return false;
			}

			// glow attenuation mode integer
			auto const gama = std::underlying_type<decltype(data1.glow_attenuation_mode)>::type(
			    data1.glow_attenuation_mode);
			auto const gamb = std::underlying_type<decltype(data2.glow_attenuation_mode)>::type(
			    data2.glow_attenuation_mode);
			if (gama < gamb) {
				return true;
			}
			if (gamb > gama) {
				return false;
			}

			if (data1.glow_half_distance < data2.glow_half_distance) {
				return true;
			}
			if (data2.glow_half_distance < data1.glow_half_distance) {
				return false;
			}

			return false;
		}

		void calculate_normals(mesh_t& mesh) {
			for (auto& vert : mesh.verts) {
				vert.normal = glm::vec3(0);
			}

			for (std::size_t i = 0; i < mesh.indices.size(); i += 3) {
				auto const a = mesh.indices[i + 0];
				auto const b = mesh.indices[i + 1];
				auto const c = mesh.indices[i + 2];

				auto& avert = mesh.verts[a];
				auto& bvert = mesh.verts[b];
				auto& cvert = mesh.verts[c];

				auto const normal =
				    cross(avert.position - bvert.position, cvert.position - bvert.position);

				avert.normal += normal;
				bvert.normal += normal;
				cvert.normal += normal;
			}

			for (auto& vert : mesh.verts) {
				vert.normal = normalize(vert.normal);
			}
		}

		std::size_t triangulate_faces(std::vector<std::size_t>& output_list,
		                              const std::vector<std::size_t>& input_face,
		                              bool const two_sided) {
			if (input_face.size() < 3) {
				return 0;
			}

			auto const face_count = (input_face.size() - 2) * (two_sided ? 2 : 1);
			auto const index_count = face_count * 3;

			output_list.reserve(output_list.size() + index_count);

			for (std::size_t i = 2; i < input_face.size(); ++i) {
				output_list.emplace_back(input_face[0]);
				output_list.emplace_back(input_face[i - 1]);
				output_list.emplace_back(input_face[i - 0]);

				if (two_sided) {
					output_list.emplace_back(input_face[i - 0]);
					output_list.emplace_back(input_face[i - 1]);
					output_list.emplace_back(input_face[0]);
				}
			}

			return face_count;
		}

		std::vector<vertex_t> shrink_vertex_list(const std::vector<vertex_t>& vertices,
		                                         std::vector<std::size_t>& indices) {
			std::vector<std::size_t> translation(vertices.size(), 0);
			std::vector<bool> use_vertex(vertices.size(), false);

			// mark all that are used
			for (auto& index : indices) {
				use_vertex[index] = true;
			}

			std::vector<vertex_t> new_vertex_list;

			// create new vertex list and translations
			std::size_t new_index = 0;
			for (std::size_t i = 0; i < vertices.size(); ++i) {
				if (use_vertex[i]) {
					new_vertex_list.emplace_back(vertices[i]);
					translation[i] = new_index;
					new_index += 1;
				}
			}

			// apply translations
			for (auto& index : indices) {
				index = translation[index];
			}

			return new_vertex_list;
		}
	} // namespace

	void instructions::parsed_csv_object_builder::add_mesh_builder() {
		// sort to keep faces with identical traits together
		std::sort(untriangulated_faces.begin(), untriangulated_faces.end(), sort_function);

		// check for an empty mesh
		if (untriangulated_faces.empty()) {
			return;
		}

		// find the next mesh with different traits
		// then copy turn all the inbetween meshes into internal format
		auto begin = untriangulated_faces.begin();
		auto const end = untriangulated_faces.end();
		while (true) {
			auto const cmp_func = [&begin](const untriangulated_face_t& face) -> bool {
				auto const tex_same = begin->data.texture == face.data.texture;
				auto const dtc_same =
				    begin->data.decal_transparent_color == face.data.decal_transparent_color;
				auto const hdtc_same = begin->data.has_decal_transparent_color
				                       == face.data.has_decal_transparent_color;
				auto const blend_same = begin->data.blend_mode == face.data.blend_mode;
				auto const glow_same =
				    begin->data.glow_attenuation_mode == face.data.glow_attenuation_mode;
				auto const glow_half =
				    begin->data.glow_half_distance == face.data.glow_half_distance;

				return !(tex_same && dtc_same && hdtc_same && blend_same && glow_same && glow_half);
			};

			auto const next_face = std::find_if(begin, end, cmp_func);

			mesh_t mesh;
			dependencies::texture_t tex;

			// apply properties to the texture
			tex.file = begin->data.texture;
			tex.decal_transparent_color = begin->data.decal_transparent_color;
			tex.has_transparent_color = begin->data.has_decal_transparent_color;

			// properties that must be the same for all faces in an internal
			// mesh
			mesh.texture = tex;
			mesh.blend_mode = begin->data.blend_mode;
			mesh.glow_attenuation_mode = begin->data.glow_attenuation_mode;
			mesh.glow_half_distance = begin->data.glow_half_distance;
			mesh.texture = tex;

			// Add faces and apply properties that can change per face
			std::for_each(begin, next_face, [&mesh](untriangulated_face_t& face) {
				auto const count =
				    triangulate_faces(mesh.indices, face.indices, face.data.back_visible);
				for (std::size_t i = 0; i < count; ++i) {
					mesh.face_data.emplace_back(
					    face_data_t{face.data.color, face.data.emissive_color});
				}
			});

			// make a shorter list of vertices and adjust the indices
			// accordingly
			mesh.verts = shrink_vertex_list(vertices, mesh.indices);

			calculate_normals(mesh);

			pso.meshes.emplace_back(std::move(mesh));
			pso.dependencies.textures.insert(std::move(tex));

			if (next_face != end) {
				begin = next_face;
			}
			else {
				break;
			}
		}

		vertices.clear();
		untriangulated_faces.clear();
	}

	void instructions::parsed_csv_object_builder::operator()(const error& arg) {
		add_error(pso.errors, arg.line, arg.cause);
	}

	void instructions::parsed_csv_object_builder::operator()(const create_mesh_builder& arg) {
		(void) arg;
		add_mesh_builder();
	}

	void instructions::parsed_csv_object_builder::operator()(const add_vertex& arg) {
		vertices.emplace_back(
		    vertex_t{glm::vec3{arg.v_x, arg.v_y, arg.v_z}, glm::vec3{arg.n_x, arg.n_y, arg.n_z}});
	}

	void instructions::parsed_csv_object_builder::operator()(const add_face& arg) {
		// Create the face
		untriangulated_faces.emplace_back();
		for (auto& vert : arg.vertices) {
			if (vert >= vertices.size()) {
				std::ostringstream error_msg;
				error_msg << "AddFace index " << vert << " is larger than the valid range: [0, "
				          << vertices.size() - 1 << "]";
				add_error(pso.errors, arg.line, error_msg.str());
			}
			else {
				untriangulated_faces.back().indices.emplace_back(vert);
			}
		}
		untriangulated_faces.back().data.back_visible = arg.side_count == sides::two;
	}

	void instructions::parsed_csv_object_builder::operator()(const cube& arg) {
		// http://openbve-project.net/documentation/HTML/object_cubecylinder.html
		// Pre cube size
		auto const v = vertices.size();

		// Create vertices
		vertices.emplace_back(
		    vertex_t{glm::vec3(arg.half_width, arg.half_height, -arg.half_depth)});
		vertices.emplace_back(
		    vertex_t{glm::vec3(arg.half_width, -arg.half_height, -arg.half_depth)});
		vertices.emplace_back(
		    vertex_t{glm::vec3(-arg.half_width, -arg.half_height, -arg.half_depth)});
		vertices.emplace_back(
		    vertex_t{glm::vec3(-arg.half_width, arg.half_height, -arg.half_depth)});
		vertices.emplace_back(vertex_t{glm::vec3(arg.half_width, arg.half_height, arg.half_depth)});
		vertices.emplace_back(
		    vertex_t{glm::vec3(arg.half_width, -arg.half_height, arg.half_depth)});
		vertices.emplace_back(
		    vertex_t{glm::vec3(-arg.half_width, -arg.half_height, arg.half_depth)});
		vertices.emplace_back(
		    vertex_t{glm::vec3(-arg.half_width, arg.half_height, arg.half_depth)});

		// Create faces
		untriangulated_faces.emplace_back(
		    untriangulated_face_t{std::vector<std::size_t>{v + 0, v + 1, v + 2, v + 3}, {}});
		untriangulated_faces.emplace_back(
		    untriangulated_face_t{std::vector<std::size_t>{v + 0, v + 4, v + 5, v + 1}, {}});
		untriangulated_faces.emplace_back(
		    untriangulated_face_t{std::vector<std::size_t>{v + 0, v + 3, v + 7, v + 4}, {}});
		untriangulated_faces.emplace_back(
		    untriangulated_face_t{std::vector<std::size_t>{v + 6, v + 5, v + 4, v + 7}, {}});
		untriangulated_faces.emplace_back(
		    untriangulated_face_t{std::vector<std::size_t>{v + 6, v + 7, v + 3, v + 2}, {}});
		untriangulated_faces.emplace_back(
		    untriangulated_face_t{std::vector<std::size_t>{v + 6, v + 2, v + 1, v + 5}, {}});
	}

	void instructions::parsed_csv_object_builder::operator()(const cylinder& arg) {
		// http://openbve-project.net/documentation/HTML/object_cubecylinder.html

		// Pre Cylinder size
		auto const v = vertices.size();

		// convert args to format used in above documentation
		auto& n = arg.sides;
		auto& r1 = arg.upper_radius;
		auto& r2 = arg.lower_radius;
		auto& h = arg.height;

		// Add vertices
		for (std::size_t i = 0; i < n; ++i) {
			vertices.emplace_back(
			    vertex_t{glm::vec3{std::cos(2 * static_cast<float>(M_PI) * static_cast<float>(i)
			                                / static_cast<float>(n))
			                           * r1, //
			                       h / 2,    //
			                       std::sin(2 * static_cast<float>(M_PI) * static_cast<float>(i)
			                                / static_cast<float>(n))
			                           * r1}});

			vertices.emplace_back(
			    vertex_t{glm::vec3{std::cos(2 * static_cast<float>(M_PI) * static_cast<float>(i)
			                                / static_cast<float>(n))
			                           * r2, //
			                       -h / 2,   //
			                       std::sin(2 * static_cast<float>(M_PI) * static_cast<float>(i)
			                                / static_cast<float>(n))
			                           * r2}});
		}

		// Add Faces
		for (std::size_t i = 0; i < n; ++i) {
			if (i != n - 1) {
				untriangulated_faces.emplace_back(
				    untriangulated_face_t{std::vector<std::size_t>{v + (2 * i + 2), v + (2 * i + 3),
				                                                   v + (2 * i + 1),
				                                                   v + (2 * i + 0)},
				                          {}});
			}
			else {
				untriangulated_faces.emplace_back(
				    untriangulated_face_t{std::vector<std::size_t>{v + 0, v + 1, v + (2 * i + 1),
				                                                   v + (2 * i + 0)},
				                          {}});
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const translate& arg) {
		auto const direction = glm::vec3(arg.x, arg.y, arg.z);
		for (auto& vertex : vertices) {
			vertex.position += direction;
		}

		// apply to all previous vertices
		if (arg.applies_to == apply_to::all_meshes) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position += direction;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const scale& arg) {
		auto const factor = glm::vec3(arg.x, arg.y, arg.z);
		for (auto& vertex : vertices) {
			vertex.position *= factor;
		}

		// apply to all previous vertices
		if (arg.applies_to == apply_to::all_meshes) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position *= factor;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const rotate& arg) {
		auto axis = glm::vec3(arg.x, arg.y, arg.z);
		if (axis == glm::vec3(0)) {
			axis = glm::vec3(1, 0, 0);
		}
		axis = normalize(axis);

		for (auto& vertex : vertices) {
			vertex.position = glm::rotate(vertex.position, arg.angle, axis);
		}

		// apply to all previous vertices
		if (arg.applies_to == apply_to::all_meshes) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position = glm::rotate(vertex.position, arg.angle, axis);
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const shear& arg) {
		auto const daxis = glm::vec3(arg.d_x, arg.d_y, arg.d_z);
		auto const saxis = glm::vec3(arg.s_x, arg.s_y, arg.s_z);

		for (auto& vertex : vertices) {
			auto const scale = arg.r * compAdd(daxis * vertex.position);
			vertex.position += saxis * scale;
		}

		// apply to all previous vertices
		if (arg.applies_to == apply_to::all_meshes) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					auto const scale = arg.r * compAdd(daxis * vertex.position);
					vertex.position += saxis * scale;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(mirror const& arg) {
		auto const factor = glm::vec3(arg.x ? -1 : 1, arg.y ? -1 : 1, arg.z ? -1 : 1);

		for (auto& vertex : vertices) {
			vertex.position *= factor;
		}

		// apply to all previous vertices
		if (arg.applies_to == apply_to::all_meshes) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position *= factor;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const set_color& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.color = core::datatypes::color8_rgba{arg.red, arg.green, arg.blue, arg.alpha};
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const set_emissive_color& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.emissive_color = core::datatypes::color8_rgb{arg.red, arg.green, arg.blue};
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const set_blend_mode& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.blend_mode = arg.blend_mode;
			face.data.glow_half_distance = arg.glow_half_distance;
			face.data.glow_attenuation_mode = arg.glow_attenuation_mode;
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const load_texture& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.texture = arg.daytime_texture;
		}
	}

	void instructions::parsed_csv_object_builder::operator()(
	    const set_decal_transparent_color& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.decal_transparent_color =
			    core::datatypes::color8_rgb{arg.red, arg.green, arg.blue};
		}
	}
	void instructions::parsed_csv_object_builder::operator()(const set_texture_coordinates& arg) {
		if (arg.vertex_index >= vertices.size()) {
			std::ostringstream error_msg;
			error_msg << "SetTextureCoordinates index " << arg.vertex_index
			          << " is larger than the valid range: [0, " << vertices.size() - 1 << "]";
			add_error(pso.errors, arg.line, error_msg.str());
		}
		else {
			vertices[arg.vertex_index].texture_coord = glm::vec2{arg.x, arg.y};
		}
	}

	parsed_b3d_csv_object_t run_csv_instructions(const instruction_list& ilist) {
		instructions::parsed_csv_object_builder pcsvob;
		for (auto& inst : ilist) {
			apply_visitor(pcsvob, inst);
		}
		// Add final mesh builder
		pcsvob.add_mesh_builder();
		return pcsvob.pso;
	}
} // namespace parsers::b3d_csv_object
} // namespace parsers
