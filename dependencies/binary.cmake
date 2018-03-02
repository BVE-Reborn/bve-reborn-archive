##########
# config #
##########

if (UNIX)
    set(Boost_USE_STATIC_LIBS On)
else()
    set(Boost_USE_STATIC_LIBS Off)
endif()

find_package(Boost 1.44 REQUIRED COMPONENTS filesystem)
find_package(Threads REQUIRED)

################
# Boost Config #
################

if (MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Boost_USE_DEBUG_RUNTIME On)
        file(GLOB BOOST_BINARY_FILES "${Boost_LIBRARY_DIRS}/*-mt-gd-${Boost_LIB_VERSION}.dll")
    else ()
        set(Boost_USE_DEBUG_RUNTIME Off)
        file(GLOB BOOST_BINARY_FILES "${Boost_LIBRARY_DIRS}/*-mt-${Boost_LIB_VERSION}.dll")
    endif()

    set_property(TARGET Boost::system APPEND PROPERTY
        INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_DYN_LINK"
    )

    set_property(TARGET Boost::filesystem APPEND PROPERTY
        INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_DYN_LINK"
    )

    file(COPY ${BOOST_BINARY_FILES} DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()
