###############
# libbve-core #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_library(bve-core SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-core PUBLIC include)
target_link_libraries(bve-core PRIVATE options::fast_math)
target_link_libraries(bve-core PUBLIC ccq::concurrentqueue mapbox::variant bve-util vtune::vtune)
add_sanitizers(bve-core)
format(bve-core)
cotire(bve-core)

set_property(TARGET bve-core PROPERTY FOLDER lib)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

####################
# libbve-core-test #
####################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_library(bve-core-test OBJECT ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(bve-core-test PRIVATE options::fast_math)
target_link_libraries(bve-core-test PUBLIC bve-core bve-util)
target_link_libraries(bve-core-test PUBLIC Threads::Threads doctest::doctest)

add_sanitizers(bve-core-test)
format(bve-core-test)
cotire(bve-core-test)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_HEADERS} ${TEST_SOURCES})

set_property(TARGET bve-core-test PROPERTY FOLDER lib/tests)
