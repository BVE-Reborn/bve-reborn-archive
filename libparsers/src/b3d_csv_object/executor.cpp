#include "b3d_csv_object.hpp"
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

namespace parsers {
namespace b3d_csv_object {
	namespace {
		// Following
		// https://stackoverflow.com/questions/3574680/sort-based-on-multiple-things-in-c
		// Sort based on properties to get all faces with same properties next
		// to each other
		static bool sort_function(const instructions::parsed_csv_object_builder::untriangulated_face_t& face1,
		                          const instructions::parsed_csv_object_builder::untriangulated_face_t& face2) {
			auto& data1 = face1.data;
			auto& data2 = face2.data;

			if (data1.texture < data2.texture)
				return true;
			if (data2.texture > data1.texture)
				return false;

			// Need to create an order, though the order doesn't matter as long
			// as they are not equal
			auto dtc_equal = data1.decal_transparent_color == data2.decal_transparent_color;
			auto dtca = glm::compAdd(data1.decal_transparent_color);
			auto dtcb = glm::compAdd(data2.decal_transparent_color);
			if (!dtc_equal && dtca < dtcb)
				return true;
			if (!dtc_equal && dtca >= dtcb)
				return false;

			if (data1.has_decal_transparent_color < data2.has_decal_transparent_color)
				return true;
			if (data2.has_decal_transparent_color > data1.has_decal_transparent_color)
				return false;

			// blend mode integer
			auto bmia = std::underlying_type<decltype(data1.BlendMode)>::type(data1.BlendMode);
			auto bmib = std::underlying_type<decltype(data2.BlendMode)>::type(data2.BlendMode);
			if (bmia < bmib)
				return true;
			if (bmib > bmia)
				return false;

			// glow attenuation mode integer
			auto gama = std::underlying_type<decltype(data1.GlowAttenuationMode)>::type(data1.GlowAttenuationMode);
			auto gamb = std::underlying_type<decltype(data2.GlowAttenuationMode)>::type(data2.GlowAttenuationMode);
			if (gama < gamb)
				return true;
			if (gamb > gama)
				return false;

			if (data1.GlowHalfDistance < data2.GlowHalfDistance)
				return true;
			if (data2.GlowHalfDistance < data1.GlowHalfDistance)
				return false;

			return false;
		}

		static void calculate_normals(mesh_t& mesh) {
			for (auto& vert : mesh.verts) {
				vert.normal = glm::vec3(0);
			}

			for (std::size_t i = 0; i < mesh.indices.size(); i += 3) {
				auto a = mesh.indices[i + 0];
				auto b = mesh.indices[i + 1];
				auto c = mesh.indices[i + 2];

				auto& avert = mesh.verts[a];
				auto& bvert = mesh.verts[b];
				auto& cvert = mesh.verts[c];

				auto normal = glm::cross(avert.normal - bvert.normal, cvert.normal - bvert.normal);

				avert.normal += normal;
				bvert.normal += normal;
				cvert.normal += normal;
			}

			for (auto& vert : mesh.verts) {
				vert.normal = glm::normalize(vert.normal);
			}
		}

		static std::size_t triangulate_faces(std::vector<std::size_t>& output_list,
		                                     const std::vector<std::size_t>& input_face,
		                                     bool two_sided) {
			if (input_face.size() < 3) {
				return 0;
			}

			auto face_count = (input_face.size() - 2) * (two_sided ? 2 : 1);
			auto index_count = face_count * 3;

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

		static std::vector<vertex_t> shrink_vertex_list(const std::vector<vertex_t>& vertices,
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
		auto end = untriangulated_faces.end();
		while (true) {
			auto cmp_func = [&begin](const untriangulated_face_t& face) -> bool {
				bool tex_same = begin->data.texture == face.data.texture;
				bool dtc_same = begin->data.decal_transparent_color == face.data.decal_transparent_color;
				bool hdtc_same = begin->data.has_decal_transparent_color == face.data.has_decal_transparent_color;
				bool blend_same = begin->data.BlendMode == face.data.BlendMode;
				bool glow_same = begin->data.GlowAttenuationMode == face.data.GlowAttenuationMode;
				bool glow_half = begin->data.GlowHalfDistance == face.data.GlowHalfDistance;

				return !(tex_same && dtc_same && hdtc_same && blend_same && glow_same && glow_half);
			};

			auto next_face = std::find_if(begin, end, cmp_func);

			mesh_t mesh;
			dependencies::texture_t tex;

			// apply properties to the texture
			tex.file = begin->data.texture;
			tex.decal_transparent_color = begin->data.decal_transparent_color;
			tex.has_transparent_color = begin->data.has_decal_transparent_color;

			// properties that must be the same for all faces in an internal
			// mesh
			mesh.texture = tex;
			mesh.BlendMode = begin->data.BlendMode;
			mesh.GlowAttenuationMode = begin->data.GlowAttenuationMode;
			mesh.GlowHalfDistance = begin->data.GlowHalfDistance;
			mesh.texture = tex;

			// Add faces and apply properties that can change per face
			std::for_each(begin, next_face, [&mesh](untriangulated_face_t& face) {
				auto count = triangulate_faces(mesh.indices, face.indices, face.data.back_visible);
				for (std::size_t i = 0; i < count; ++i) {
					mesh.face_data.emplace_back(face_data_t{face.data.color, face.data.emissive_color});
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

	void instructions::parsed_csv_object_builder::operator()(const Error& arg) {
		errors::add_error(pso.errors, arg.line, arg.cause);
	}

	void instructions::parsed_csv_object_builder::operator()(const CreateMeshBuilder& arg) {
		(void) arg;
		add_mesh_builder();
	}

	void instructions::parsed_csv_object_builder::operator()(const AddVertex& arg) {
		vertices.emplace_back(vertex_t{glm::vec3{arg.vX, arg.vY, arg.vZ}, glm::vec3{arg.nX, arg.nY, arg.nZ}});
	}

	void instructions::parsed_csv_object_builder::operator()(const AddFace& arg) {
		// Create the face
		untriangulated_faces.emplace_back();
		for (auto& vert : arg.vertices) {
			if (vert >= vertices.size()) {
				std::ostringstream error_msg;
				error_msg << "AddFace index " << vert << " is larger than the valid range: [0, " << vertices.size() - 1
				          << "]";
				errors::add_error(pso.errors, arg.line, error_msg.str());
			}
			else {
				untriangulated_faces.back().indices.emplace_back(vert);
			}
		}
		untriangulated_faces.back().data.back_visible = arg.two;
	}

	void instructions::parsed_csv_object_builder::operator()(const Cube& arg) {
		// http://openbve-project.net/documentation/HTML/object_cubecylinder.html
		// Pre cube size
		auto v = vertices.size();

		// Create vertices
		vertices.emplace_back(vertex_t{glm::vec3(arg.HalfWidth, arg.HalfHeight, -arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(arg.HalfWidth, -arg.HalfHeight, -arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(-arg.HalfWidth, -arg.HalfHeight, -arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(-arg.HalfWidth, arg.HalfHeight, -arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(arg.HalfWidth, arg.HalfHeight, arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(arg.HalfWidth, -arg.HalfHeight, arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(-arg.HalfWidth, -arg.HalfHeight, arg.HalfDepth)});
		vertices.emplace_back(vertex_t{glm::vec3(-arg.HalfWidth, arg.HalfHeight, arg.HalfDepth)});

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

	void instructions::parsed_csv_object_builder::operator()(const Cylinder& arg) {
		// http://openbve-project.net/documentation/HTML/object_cubecylinder.html

		// Pre Cylinder size
		auto v = vertices.size();

		// convert args to format used in above documentation
		auto& n = arg.sides;
		auto& r1 = arg.UpperRadius;
		auto& r2 = arg.LowerRadius;
		auto& h = arg.Height;

		// Add vertices
		for (std::size_t i = 0; i < n; ++i) {
			vertices.emplace_back(vertex_t{glm::vec3{std::cos(2 * float(M_PI) * float(i) / float(n)) * r1, //
			                                         h / 2,                                                //
			                                         std::sin(2 * float(M_PI) * float(i) / float(n)) * r1}});

			vertices.emplace_back(vertex_t{glm::vec3{std::cos(2 * float(M_PI) * float(i) / float(n)) * r2, //
			                                         -h / 2,                                               //
			                                         std::sin(2 * float(M_PI) * float(i) / float(n)) * r2}});
		}

		// Add Faces
		for (std::size_t i = 0; i < n; ++i) {
			if (i != n - 1) {
				untriangulated_faces.emplace_back(
				    untriangulated_face_t{std::vector<std::size_t>{v + (2 * i + 2), v + (2 * i + 3), v + (2 * i + 1),
				                                                   v + (2 * i + 0)},
				                          {}});
			}
			else {
				untriangulated_faces.emplace_back(
				    untriangulated_face_t{std::vector<std::size_t>{v + 0, v + 1, v + (2 * i + 1), v + (2 * i + 0)},
				                          {}});
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const Translate& arg) {
		glm::vec3 direction = glm::vec3(arg.X, arg.Y, arg.Z);
		for (auto& vertex : vertices) {
			vertex.position += direction;
		}

		// apply to all previous vertices
		if (arg.all) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position += direction;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const Scale& arg) {
		glm::vec3 factor = glm::vec3(arg.X, arg.Y, arg.Z);
		for (auto& vertex : vertices) {
			vertex.position *= factor;
		}

		// apply to all previous vertices
		if (arg.all) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position *= factor;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const Rotate& arg) {
		glm::vec3 axis = glm::vec3(arg.X, arg.Y, arg.Z);
		if (axis == glm::vec3(0)) {
			axis = glm::vec3(1, 0, 0);
		}
		axis = glm::normalize(axis);

		for (auto& vertex : vertices) {
			vertex.position = glm::rotate(vertex.position, arg.Angle, axis);
		}

		// apply to all previous vertices
		if (arg.all) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					vertex.position = glm::rotate(vertex.position, arg.Angle, axis);
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const Shear& arg) {
		glm::vec3 daxis = glm::vec3(arg.dX, arg.dY, arg.dZ);
		glm::vec3 saxis = glm::vec3(arg.sX, arg.sY, arg.sZ);

		for (auto& vertex : vertices) {
			auto scale = arg.r * glm::compAdd(daxis * vertex.position);
			vertex.position += saxis * scale;
		}

		// apply to all previous vertices
		if (arg.all) {
			for (auto& mesh : pso.meshes) {
				for (auto& vertex : mesh.verts) {
					auto scale = arg.r * glm::compAdd(daxis * vertex.position);
					vertex.position += saxis * scale;
				}
			}
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const SetColor& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.color = openbve2::datatypes::color8_rgba{arg.Red, arg.Green, arg.Blue, arg.Alpha};
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const SetEmissiveColor& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.emissive_color = openbve2::datatypes::color8_rgb{arg.Red, arg.Green, arg.Blue};
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const SetBlendMode& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.BlendMode = arg.BlendMode;
			face.data.GlowHalfDistance = arg.GlowHalfDistance;
			face.data.GlowAttenuationMode = arg.GlowAttenuationMode;
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const LoadTexture& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.texture = arg.DaytimeTexture;
		}
	}

	void instructions::parsed_csv_object_builder::operator()(const SetDecalTransparentColor& arg) {
		for (auto& face : untriangulated_faces) {
			face.data.decal_transparent_color = openbve2::datatypes::color8_rgb{arg.Red, arg.Green, arg.Blue};
		}
	}
	void instructions::parsed_csv_object_builder::operator()(const SetTextureCoordinates& arg) {
		if (arg.VertexIndex >= vertices.size()) {
			std::ostringstream error_msg;
			error_msg << "SetTextureCoordinates index " << arg.VertexIndex << " is larger than the valid range: [0, "
			          << vertices.size() - 1 << "]";
			errors::add_error(pso.errors, arg.line, error_msg.str());
		}
		else {
			vertices[arg.VertexIndex].texture_coord = glm::vec2{arg.X, arg.Y};
		}
	}

	parsed_b3d_csv_object_t run_csv_instructions(const instruction_list& ilist) {
		instructions::parsed_csv_object_builder pcsvob;
		for (auto& inst : ilist) {
			mapbox::util::apply_visitor(pcsvob, inst);
		}
		// Add final mesh builder
		pcsvob.add_mesh_builder();
		return pcsvob.pso;
	}
} // namespace b3d_csv_object
} // namespace parsers
