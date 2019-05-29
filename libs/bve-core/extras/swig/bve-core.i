%module bve_core_python

%{
#define SWIG_FILE_WTIH_INIT
#include <core/openbve/filesystem.hpp>
#include <util/unlanguage.hpp>
%}

%include <core/openbve/filesystem.hpp>
