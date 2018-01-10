cmake_minimum_required(VERSION 3.0)
project(OpenBVE2 CXX)

set (CMAKE_CXX_STANDARD 14)
set (CXX_STANDARD_REQUIRED True)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

add_subdirectory("dependencies/glm")

set_target_properties(glm_dummy PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

include("dependencies/cmake-utils/include/clang_format.cmake")
include("dependencies/cmake-utils/include/linter.cmake")
include("dependencies/cmake-utils/include/make_target_includes_system.cmake")
include("dependencies/cmake-utils/include/warnings.cmake")

if (NOT MSVC)
	make_target_includes_system(glm)
endif()
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
# Suppress annoying boost message
add_definitions(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        add_definitions(/D_ITERATOR_DEBUG_LEVEL=1)
    endif()
else ()
    if (MSVC)
        add_compile_options(/fp:fast)
    else()
        add_compile_options(-ffast-math)
    endif()
endif()

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)

configure_linter()
