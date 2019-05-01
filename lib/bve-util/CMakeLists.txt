###############
# libbve-util #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-util SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-util PUBLIC include)
target_link_libraries(bve-util PUBLIC glm gsl::gsl foundational::foundational bve-eastl)

finish_bve_target(bve-util)

set_property(TARGET bve-util PROPERTY FOLDER lib)

##################
# bve-util-tests #
##################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_bve_library(bve-util-test OBJECT ${TEST_SOURCES})
target_include_directories(bve-util-test PRIVATE src tests)
target_link_libraries(bve-util-test PUBLIC bve-util)

finish_bve_target(bve-util-test)

set_property(TARGET bve-util-test PROPERTY FOLDER lib/tests)
