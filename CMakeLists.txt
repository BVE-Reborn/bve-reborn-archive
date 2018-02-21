cmake_minimum_required(VERSION 3.9)
project(OpenBVE2 CXX)

# Allow boost to be built from the binary dir
list(INSERT CMAKE_PREFIX_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/binary/build")

include("dependencies/cmake-utils/include/clang_format.cmake")
include("dependencies/cmake-utils/include/linter.cmake")
include("dependencies/cmake-utils/include/meta_features.cmake")
include("dependencies/cmake-utils/include/warnings.cmake")
include("dependencies/targets.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

configure_file(.clang-tidy-manager .clang-tidy-manager COPYONLY)

###########
# depends #
###########

find_package(Boost 1.44 REQUIRED COMPONENTS filesystem)
find_package(Threads REQUIRED)

################
# Boost Config #
################

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

    set_property(TARGET Boost::system APPEND PROPERTY
        INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_DYN_LINK"
    )

    set_property(TARGET Boost::filesystem APPEND PROPERTY
        INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_DYN_LINK"
    )

    file(COPY ${BOOST_BINARY_FILES} DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

if (UNIX)
    set(Boost_USE_STATIC_LIBS On)
else()
    set(Boost_USE_STATIC_LIBS Off)
endif()

#################
# Global Config #
#################

# TODO: Proper Exporting of Symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# TODO: Fix Warning Propagation
add_warnings()

add_linted_subdirectory(libcore)
add_linted_subdirectory(libparsers)

configure_linter()
