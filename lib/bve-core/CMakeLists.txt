###############
# libbve-core #
###############

file(GLOB_RECURSE ISPC_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.ispc")
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_ispc_object_library(bve-core-ispc ${ISPC_SOURCES} 
    TARGETS 
        sse2
        sse4
        avx1
        avx1.1
        avx2
        avx512knl
        avx512skx 
    64BIT_ADDRESSING 
    PIC
    HEADER_PATH ispc/
)
add_bve_library(bve-core SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-core PUBLIC include)
target_link_libraries(bve-core PUBLIC bve-core-ispc bve-util vtune::vtune glm gsl::gsl)

finish_bve_target(bve-core)

set_property(TARGET bve-core PROPERTY FOLDER lib)
set_property(TARGET bve-core-ispc PROPERTY FOLDER lib)

####################
# libbve-core-test #
####################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_bve_library(bve-core-test OBJECT ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(bve-core-test PUBLIC bve-core bve-util)
target_link_libraries(bve-core-test PUBLIC Threads::Threads)

finish_bve_target(bve-core-test)

set_property(TARGET bve-core-test PROPERTY FOLDER lib/tests)
