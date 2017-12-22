cmake_minimum_required(VERSION 3.0)
project(OpenBVE2 CXX)

set (CMAKE_CXX_STANDARD 14)
set (CXX_STANDARD_REQUIRED True)

include("dependencies/cmake-utils/include/clang_format.cmake")
include("dependencies/cmake-utils/include/linter.cmake")
include("dependencies/cmake-utils/include/make_target_includes_system.cmake")
include("dependencies/cmake-utils/include/warnings.cmake")

add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)

configure_linter()
