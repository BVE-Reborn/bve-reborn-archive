###############
# libbve-core #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-core SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-core PUBLIC include)
target_link_libraries(bve-core PUBLIC bve-util vtune::vtune)

finish_bve_target(bve-core)

set_property(TARGET bve-core PROPERTY FOLDER lib)

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
