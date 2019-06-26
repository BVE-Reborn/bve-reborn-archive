%module bve_parsers_cs

%include <inttypes.i>
%include <std_common.i>
%include <std_string.i>
%include <std_map.i>
%include <std_set.i>
%include <std_vector.i>

%{
#include <parsers/b3d_csv.hpp>
using namespace bve;
using namespace bve::parsers;
%}

// GLM Stuff
namespace glm {
	struct u8vec1 {
		uint8_t x;
		uint8_t r;
	};

	struct u8vec2 {
		uint8_t x;
		uint8_t y;
		uint8_t r;
		uint8_t g;
	};

	struct u8vec3 {
		uint8_t x;
		uint8_t y;
		uint8_t z;
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	struct u8vec4 {
		uint8_t x;
		uint8_t y;
		uint8_t z;
		uint8_t w;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	struct highp_vec1 {
		float x;
	};

	struct vec2 {
		float x;
		float y;
	};

	struct vec3 {
		float x;
		float y;
		float z;
	};
	
	struct vec4 {
		float x;
		float y;
		float z;
		float w;
	};
}

// Manually include dependencies, but only the ones we truly need
%include <util/datatypes.hpp>
%include <parsers/errors.hpp>
%include <parsers/dependencies.hpp>
%include <parsers/b3d_csv.hpp>

// All standard template instantiations we need
%template(std_set_Texture) std::set<bve::parsers::dependencies::Texture>;
%template(std_map_MultiError) std::map<std::string, bve::parsers::errors::Errors>;
%template(std_vector_FaceData) std::vector<bve::parsers::b3d_csv_object::FaceData>;
%template(std_vector_MeshData) std::vector<bve::parsers::b3d_csv_object::Mesh>;
%template(std_vector_VertexData) std::vector<bve::parsers::b3d_csv_object::Vertex>;
%template(std_vector_size_t) std::vector<std::size_t>;
%template(std_vector_Error) std::vector<bve::parsers::errors::Error>;
