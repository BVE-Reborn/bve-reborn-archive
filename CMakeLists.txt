cmake_minimum_required(VERSION 3.12)
project(BVE-Reborn C CXX)

find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
                                           ${CMAKE_SOURCE_DIR}/extern
                                           ${CMAKE_SOURCE_DIR}/extern/cmake
                                           ${CMAKE_SOURCE_DIR}/extern/cmake-utils/include
                                           ${CMAKE_SOURCE_DIR}/extern/cotire/CMake
                                           ${CMAKE_SOURCE_DIR}/extern/nova/cmake
                                           ${CMAKE_SOURCE_DIR}/tools/cmake
)

include(config)

if(NOT BVEREBORN_PACKAGE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS False)

if (BVEREBORN_WITH_FUZZING)
    add_compile_options(-fsanitize=fuzzer-no-link)
endif()

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release) 
endif() 

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

include(ClangFormat)
include(ClangTidy)

include(extern)
include(meta_features)
include(nova)
include(warnings)

if (NOT BVEREBORN_WIN)
    macro(bve_cotire)
        cotire(${ARGN})
    endmacro()
else()
    macro(bve_cotire)
    endmacro()
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY COTIRE_PREFIX_HEADER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/dependencies)
set_directory_properties(PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

find_package(VTune)

#################
# Global Config #
#################

if(EXISTS ${CMAKE_BINARY_DIR}/BVE-Reborn.sln.DotSettings)
    file(COPY ${CMAKE_BINARY_DIR}/BVE-Reborn.sln.DotSettings DESTINATION tools/ide)
else()
    file(COPY tools/ide/BVE-Reborn.sln.DotSettings DESTINATION ${CMAKE_BINARY_DIR})
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

add_subdirectory(lib/bve-core)
add_subdirectory(lib/bve-log)
add_subdirectory(lib/bve-parsers)
add_subdirectory(lib/bve-util)
add_subdirectory(src/test-runner)

if(TARGET format)
    set_property(TARGET format PROPERTY FOLDER tools)
endif()
if(TARGET lint)
    set_property(TARGET lint PROPERTY FOLDER tools)
endif()
if(TARGET lint-fix)
    set_property(TARGET lint-fix PROPERTY FOLDER tools)
endif()

if(TARGET clean_cotire)
    set_property(TARGET clean_cotire PROPERTY FOLDER tools)
endif()
