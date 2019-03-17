cmake_minimum_required(VERSION 3.12)
project(BVE-Reborn C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (VCPKG_TOOLCHAIN)
    include_directories(SYSTEM "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
endif()

option(BVEREBORN_PACKAGE "Enable if building from a package manager. Does the minimal needed configuration." Off)
option(BVEREBORN_WITH_FUZZING "Enable to build automatic fuzzing. Experimental." Off)
option(BVEREBORN_CODE_COVERAGE "Enable to build with code coverage." Off)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" 
    "${CMAKE_SOURCE_DIR}/extern"
    "${CMAKE_SOURCE_DIR}/extern/cmake-helpers"
    "${CMAKE_SOURCE_DIR}/tools/cmake"
)

include(add_build)
include(cmake-helpers)
include(extern)
include(nova)
include(warnings)

if ((NOT CMAKE_BUILD_TYPE) AND (NOT OS_WINDOWS))
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be set on mac and linux.")
endif()

if (OS_32bit)
    message(FATAL_ERROR "BVE-Reborn does not support 32bit builds.")
endif()

if(NOT BVEREBORN_PACKAGE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS False)

if(OS_WINDOWS)
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_ITERATOR_DEBUG_LEVEL=1")
    add_compile_options(/Zi /permissive- /MP)
    set (CMAKE_SHARED_LINKER_FLAGS "/DEBUG")
    set (CMAKE_EXE_LINKER_FLAGS "/DEBUG")
else()
    add_compile_options(-g)
    link_libraries(-g)
endif()

macro(target_bve_cotire)
    if (NOT OS_WINDOWS)
        target_cotire(${ARGN})
    endif()
endmacro()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY COTIRE_PREFIX_HEADER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/extern)
set_directory_properties(PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

#################
# Global Config #
#################

if(EXISTS ${CMAKE_BINARY_DIR}/BVE-Reborn.sln.DotSettings)
    file(COPY ${CMAKE_BINARY_DIR}/BVE-Reborn.sln.DotSettings DESTINATION tools/ide)
else()
    file(COPY tools/ide/BVE-Reborn.sln.DotSettings DESTINATION ${CMAKE_BINARY_DIR})
endif()

macro(target_bve_coverage)
    if(BVEREBORN_CODE_COVERAGE)
        target_coverage(${ARGN})
    endif()
endmacro()

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(lib/bve-core)
add_subdirectory(lib/bve-log)
add_subdirectory(lib/bve-parsers)
add_subdirectory(lib/bve-util)
add_subdirectory(src/test-runner)

if(TARGET clean_cotire)
    set_property(TARGET clean_cotire PROPERTY FOLDER "")
endif()
