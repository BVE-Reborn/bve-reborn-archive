%module bve_cs

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
%template(std_set_Texture) std::set<bve::parsers::dependencies::Texture>;
%template(std_map_MultiError) std::map<std::string, bve::parsers::errors::Errors>;
%template(std_vector_FaceData) std::vector<bve::parsers::b3d_csv_object::FaceData>;
%template(std_vector_MeshData) std::vector<bve::parsers::b3d_csv_object::Mesh>;
%template(std_vector_VertexData) std::vector<bve::parsers::b3d_csv_object::Vertex>;
%template(std_vector_size_t) std::vector<std::size_t>;
%template(std_vector_Error) std::vector<bve::parsers::errors::Error>;
