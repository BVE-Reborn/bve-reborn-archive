cmake_minimum_required(VERSION 3.9)
project(OpenBVE2 CXX)

# Allow boost to be built from the binary dir
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/binary/build_x86")
else()
    list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/binary/build_x64")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(.clang-tidy-manager .clang-tidy-manager COPYONLY)

set(CMAKE_CXX_EXTENSIONS False)

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release) 
endif() 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/dependencies/cotire/CMake)
include(cotire)
include("dependencies/cmake-utils/include/clang_format.cmake")
include("dependencies/cmake-utils/include/linker_support.cmake")
include("dependencies/cmake-utils/include/linter.cmake")
include("dependencies/cmake-utils/include/meta_features.cmake")
include("dependencies/cmake-utils/include/warnings.cmake")
include("dependencies/targets.cmake")
include("dependencies/binary.cmake")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY COTIRE_PREFIX_HEADER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/dependencies)
set_directory_properties(PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

#################
# Global Config #
#################

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# TODO: Fix Warning Propagation
add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)
add_linted_subdirectory(test_main)

configure_linter()
