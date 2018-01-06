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
# Suppress annoying boost message
add_definitions(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)

#message(STATUS "${CMAKE_CXX_FLAGS}")
#if(OPENBVE2_ADD_FLAGS MATCHES "/GL")
#    string(REGEX REPLACE "/Z(i|I)" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
#endif()
#if(OPENBVE2_ADD_FLAGS MATCHES "/GL")
#    string(REGEX REPLACE "/Z(i|I)" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#endif()
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPENBVE2_ADD_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENBVE2_ADD_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OPENBVE2_LINKER_FLAGS}")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OPENBVE2_LINKER_FLAGS}")

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)

configure_linter()
