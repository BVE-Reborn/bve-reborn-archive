set (BSF_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/bsf/Source)
set (APP_ROOT_DIR ${BSF_DIRECTORY})
set (SECONDARY_APP_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set (BS_IS_BANSHEE3D 0)

set(BS_BINARY_DEP_WEBSITE "https://bsfproxy.connorwfitzgerald.com/bsf/" CACHE STRING "The location that binary dependencies will be pulled from. Must follow the same naming scheme as data.banshee3d.com")
if (NOT("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang"))
    set(ENABLE_COTIRE True CACHE BOOL  "Enable cotire's precompiled headers and unity build support (experimental).")
endif()

include(${BSF_SOURCE_DIR}/CMake/HelperMethods.cmake)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/bsf")

message(STATUS ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
