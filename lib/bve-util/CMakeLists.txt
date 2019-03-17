###############
# libbve-util #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")



add_library(bve-util SHARED ${SOURCES} ${HEADERS})
target_compile_features(bve-util PUBLIC cxx_std_14)
target_include_directories(bve-util PUBLIC include)
target_link_libraries(bve-util PRIVATE options::fast_math)
add_sanitizers(bve-util)
format(bve-util)
bve_cotire(bve-util)

set_property(TARGET bve-util PROPERTY FOLDER lib)

##################
# bve-util-tests #
##################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_library(bve-util-test OBJECT ${TEST_SOURCES})
target_include_directories(bve-util-test PRIVATE src tests)
target_link_libraries(bve-util-test PUBLIC bve-util)

set_property(TARGET bve-util-test PROPERTY FOLDER lib/tests)

add_sanitizers(bve-util-test)
format(bve-util-test)
bve_cotire(bve-util-test)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES} ${TEST_SOURCES} ${TEST_HEADERS})
