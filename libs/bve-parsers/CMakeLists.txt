
##################
# libbve-parsers #
##################

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.hpp" "include/*.hpp")

add_bve_library(bve-parsers SHARED ${SOURCES} ${HEADERS})

target_include_directories(bve-parsers PUBLIC include)
target_include_directories(bve-parsers PRIVATE src)
if(OS_WINDOWS)
    target_compile_definitions(bve-parsers PRIVATE $<$<CONFIG:Debug>:/D_ITERATOR_DEBUG_LEVEL=1>)
endif()
target_link_libraries(bve-parsers PUBLIC glm gsl::gsl unofficial::abseil::types bve-util)

finish_bve_target(bve-parsers)

set_property(TARGET bve-parsers PROPERTY FOLDER lib)

#######################
# libbve-parsers-test #
#######################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_bve_library(bve-parsers-test OBJECT ${TEST_SOURCES})
target_include_directories(bve-parsers-test PRIVATE src tests)
target_link_libraries(bve-parsers-test PUBLIC bve-parsers)
target_link_libraries(bve-parsers-test PUBLIC cppfs::cppfs)

finish_bve_target(bve-parsers-test)

set_property(TARGET bve-parsers-test PROPERTY FOLDER lib/tests)

#############
# tests_old #
#############

file(GLOB_RECURSE OLD_TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests_old/*.cpp")
file(GLOB_RECURSE OLD_TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests_old/*.hpp")

configure_file(tests_old/old_tests_config.hpp.in
               old_tests_config.hpp)

set(CMAKE_INCLUDE_CURRENT_DIR True)

add_bve_library(bve-old-tests ${OLD_TEST_SOURCES} ${OLD_TEST_HEADERS})
target_include_directories(bve-old-tests PRIVATE src)
target_link_libraries(bve-old-tests PRIVATE bve-util bve-parsers)
target_link_libraries(bve-old-tests PRIVATE Threads::Threads cppfs::cppfs)

finish_bve_target(bve-old-tests)

set_property(TARGET bve-old-tests PROPERTY FOLDER lib/tests)
