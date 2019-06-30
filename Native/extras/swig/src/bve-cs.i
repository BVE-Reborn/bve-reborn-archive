%module bve_cs

%nspace;

#pragma SWIG nowarn=503,516

%{
  #include <EABase/config/eaplatform.h>
  #ifdef EA_PLATFORM_WINDOWS
  #pragma warning(disable : 4100)
  #pragma warning(disable : 4251)
  #endif
%}

// Annoying Crap
%ignore imaxabs;
%ignore imaxdiv;
%ignore imaxdiv_t;
%ignore strtoimax;
%ignore strtoumax;

// C++ Helper Things
%include <raw/pointers.i>

// Standard Library Things
%include <builtin/std_set.i>
%include <inttypes.i>
%include <std_common.i>
%include <std_string.i>
%include <std_map.i>
%include <std_vector.i>

// External Library Things
%include <cppfs/FilePath.i>
%include <glm/glm.i>

// BVE Things
%include <core/image/loader.i>
%include <core/openbve/filesystem.i>
%include <parsers/b3d_csv.i>

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
