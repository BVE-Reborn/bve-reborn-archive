###############
# libbve-core #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

lint(FILES ${SOURCES})
format(FILES ${HEADERS} ${SOURCES})

add_library(bve-core SHARED ${SOURCES} ${HEADERS})
target_compile_features(bve-core PUBLIC cxx_std_14)
target_include_directories(bve-core PUBLIC include)
target_link_libraries(bve-core PUBLIC glm::glm mapbox::variant gsl::gsl)
target_link_libraries(bve-core PRIVATE options::fast_math)
add_sanitizers(bve-core)
cotire(bve-core)

set_property(TARGET bve-core PROPERTY FOLDER lib)

##################
# bve-core-tests #
##################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp")

add_library(bve-core-test OBJECT ${TEST_SOURCES})
target_include_directories(bve-core-test PRIVATE src tests)
target_link_libraries(bve-core-test PUBLIC bve-core)
target_link_libraries(bve-core-test PUBLIC doctest::doctest)

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})

set_property(TARGET bve-core-test PROPERTY FOLDER lib/tests)

add_sanitizers(bve-core-test)
cotire(bve-core-test)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES} ${TEST_SOURCES} ${TEST_HEADERS})
