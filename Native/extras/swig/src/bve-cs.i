%module bve_cs

%nspace;

#pragma SWIG nowarn=503

%{
#include <EABase/config/eaplatform.h>
#ifdef EA_PLATFORM_WINDOWS
#pragma warning(disable : 4100)
#endif
#include <parsers/b3d_csv.hpp>
using namespace bve;
using namespace bve::parsers;
%}

%ignore operator <;
%ignore operator <<;

%include <raw/pointers.i>

%include <builtin/std_set.i>
%include <inttypes.i>
%include <std_common.i>
%include <std_string.i>
%include <std_map.i>
%include <std_vector.i>

%include <glm/glm.i>

// Manually include dependencies, but only the ones we truly need
%include <util/datatypes.hpp>
%include <parsers/errors.hpp>
%include <parsers/dependencies.hpp>
%include <parsers/b3d_csv.hpp>

// All standard template instantiations we need
namespace std {
	%template(set_Texture) std::set<bve::parsers::dependencies::Texture>;
	%template(map_MultiError) std::map<std::string, bve::parsers::errors::Errors>;
	%template(vector_FaceData) std::vector<bve::parsers::b3d_csv_object::FaceData>;
	%template(vector_MeshData) std::vector<bve::parsers::b3d_csv_object::Mesh>;
	%template(vector_VertexData) std::vector<bve::parsers::b3d_csv_object::Vertex>;
	%template(vector_size_t) std::vector<std::size_t>;
	%template(vector_Error) std::vector<bve::parsers::errors::Error>;
}
