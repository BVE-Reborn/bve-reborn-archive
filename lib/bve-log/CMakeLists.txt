##############
# libbve-log #
##############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")



add_library(bve-log SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-log PUBLIC include)
target_link_libraries(bve-log PRIVATE options::fast_math)
target_link_libraries(bve-log PUBLIC fmt::fmt bve-core)
add_sanitizers(bve-log)
format(bve-log)
cotire(bve-log)

set_property(TARGET bve-log PROPERTY FOLDER lib)

###################
# libbve-log-test #
###################

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp")

add_library(bve-log-test OBJECT ${TEST_SOURCES} ${TEST_HEADERS})
target_include_directories(bve-log-test PUBLIC ../bve-parsers/src)
target_link_libraries(bve-log-test PUBLIC bve-log bve-core)
target_link_libraries(bve-log-test PUBLIC Threads::Threads gsl::gsl)

add_sanitizers(bve-log-test)
format(bve-log-test)
cotire(bve-log-test)


source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${TEST_HEADERS} ${TEST_SOURCES})

set_property(TARGET bve-log-test PROPERTY FOLDER lib/tests)
