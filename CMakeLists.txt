cmake_minimum_required(VERSION 3.0)
project(OpenBVE2 CXX)

set (CMAKE_CXX_STANDARD 14)
set (CXX_STANDARD_REQUIRED True)

add_subdirectory("dependencies/glm")

include("dependencies/cmake-utils/include/clang_format.cmake")
include("dependencies/cmake-utils/include/linter.cmake")
include("dependencies/cmake-utils/include/make_target_includes_system.cmake")
include("dependencies/cmake-utils/include/warnings.cmake")

if (NOT MSVC)
	make_target_includes_system(glm)
endif()
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)

configure_linter()
