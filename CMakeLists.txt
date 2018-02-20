cmake_minimum_required(VERSION 3.0)
project(OpenBVE2 CXX)

set (CMAKE_CXX_STANDARD 14)
set (CXX_STANDARD_REQUIRED True)

list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/binary/build")

include("dependencies/cmake-utils/include/clang_format.cmake")
include("dependencies/cmake-utils/include/linter.cmake")
include("dependencies/cmake-utils/include/make_target_includes_system.cmake")
include("dependencies/cmake-utils/include/warnings.cmake")

configure_file(.clang-tidy-manager .clang-tidy-manager COPYONLY)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

add_subdirectory("dependencies/glm")

set_target_properties(glm_dummy PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)


if (NOT MSVC)
	make_target_includes_system(glm)
endif()
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        add_definitions(/D_ITERATOR_DEBUG_LEVEL=1)
    endif()
else ()
    if (MSVC)
        add_compile_options(/fp:fast)
    else()
        add_compile_options(-ffast-math)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_definitions("-D__extern_always_inline=extern __attribute__((always_inline)) inline")
        endif()
    endif()
endif()

# Copy boost binary files
if (MSVC)
    if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(Boost_USE_DEBUG_RUNTIME On)
            file(GLOB BOOST_BINARY_FILES "../windep32/bin_debug/*")
        else ()
            set(Boost_USE_DEBUG_RUNTIME Off)
            file(GLOB BOOST_BINARY_FILES "../windep32/bin/*")
        endif()
    else()
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(Boost_USE_DEBUG_RUNTIME On)
            file(GLOB BOOST_BINARY_FILES "../windep64/bin_debug/*")
        else ()
            set(Boost_USE_DEBUG_RUNTIME Off)
            file(GLOB BOOST_BINARY_FILES "../windep64/bin/*")
        endif()
    endif()
    add_definitions(-DBOOST_ALL_DYN_LINK)
    file(COPY ${BOOST_BINARY_FILES} DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

if (UNIX)
    set(Boost_USE_STATIC_LIBS On)
else()
    set(Boost_USE_STATIC_LIBS Off)
endif()
find_package(Boost 1.44 REQUIRED COMPONENTS filesystem)
find_package(Threads REQUIRED)

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)

configure_linter()
