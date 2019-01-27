cmake_minimum_required(VERSION 3.12)
project(BVE-Reborn C CXX)

find_package(Threads REQUIRED)

include(util/cmake/config.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

configure_file(.clang-tidy-manager .clang-tidy-manager COPYONLY)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS False)

if (BVEREBORN_WITH_FUZZING)
    add_compile_options(-fsanitize=fuzzer-no-link)
endif()

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release) 
endif() 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/extern/cotire/CMake
                                           ${CMAKE_SOURCE_DIR}/extern/cmake
                                           ${CMAKE_SOURCE_DIR}/extern/cmake-utils/include
                                           ${CMAKE_SOURCE_DIR}/extern)

if(BVEREBORN_WIN)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_ITERATOR_DEBUG_LEVEL=1")
    add_compile_options(/Zi /permissive- /MP)
    set (CMAKE_SHARED_LINKER_FLAGS "/DEBUG")
    set (CMAKE_EXE_LINKER_FLAGS "/DEBUG")
else()
    add_compile_options(-g)
    link_libraries(-g)
endif()

include(cotire)

include(clang_format)
include(extern)
include(linker_support)
include(linter)
include(meta_features)
include(nova)
include(warnings)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY COTIRE_PREFIX_HEADER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/dependencies)
set_directory_properties(PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

#################
# Global Config #
#################

if(EXISTS ${CMAKE_BINARY_DIR}/BVE-Reborn.sln.DotSettings)
    file(COPY ${CMAKE_BINARY_DIR}/BVE-Reborn.sln.DotSettings DESTINATION ${CMAKE_SOURCE_DIR})
else()
    file(COPY BVE-Reborn.sln.DotSettings DESTINATION ${CMAKE_BINARY_DIR})
endif()

if((${CMAKE_BUILD_TYPE} MATCHES "Debug") AND (BVEREBORN_CODE_COVERAGE))
    message(STATUS "Compiling with CodeCoverage")
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
else()
    message(STATUS "Compiling without CodeCoverage")
endif()

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# TODO: Fix Warning Propagation
add_warnings()

add_linted_subdirectory(lib/bve-core)
add_linted_subdirectory(lib/bve-log)
add_linted_subdirectory(lib/bve-parsers)
add_linted_subdirectory(src/test-runner)

configure_linter()

if(TARGET format)
    set_property(TARGET format PROPERTY FOLDER tools)
endif()
if(TARGET lint)
    set_property(TARGET lint PROPERTY FOLDER tools)
endif()
if(TARGET lint-fix)
    set_property(TARGET lint-fix PROPERTY FOLDER tools)
endif()

