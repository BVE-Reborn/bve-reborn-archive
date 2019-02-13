##################
# libbve-parsers #
##################

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")



add_library(bve-parsers SHARED ${SOURCES} ${HEADERS})

target_include_directories(bve-parsers PUBLIC include)
target_link_libraries(bve-parsers PUBLIC bve-core)
target_link_libraries(bve-parsers PUBLIC mapbox::variant gsl::gsl absl::optional)

target_include_directories(bve-parsers PRIVATE src)
target_link_libraries(bve-parsers PRIVATE rapidxml::rapidxml options::fast_math)

set_property(TARGET bve-parsers PROPERTY FOLDER lib)

add_sanitizers(bve-parsers)
format(bve-parsers)
cotire(bve-parsers)

#######################
# libbve-parsers-test #
#######################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp")

add_library(bve-parsers-test OBJECT ${TEST_SOURCES})
target_include_directories(bve-parsers-test PRIVATE src tests)
target_link_libraries(bve-parsers-test PUBLIC bve-parsers)
target_link_libraries(bve-parsers-test PUBLIC doctest::doctest cppfs::cppfs)



set_property(TARGET bve-parsers-test PROPERTY FOLDER lib/tests)

add_sanitizers(bve-parsers-test)
format(bve-parsers-test)
cotire(bve-parsers-test)

#if (BVEREBORN_WITH_FUZZING)
#    add_executable(fuzz_libparsers_function_scripts tests/function_scripts/fuzzer.cpp)
#    target_compile_options(fuzz_libparsers_function_scripts PRIVATE -fsanitize=fuzzer)
#    target_link_libraries(fuzz_libparsers_function_scripts PRIVATE parsers options::iterator_debug_level_1 -fsanitize=fuzzer)
#    
#    format(FILES tests/function_scripts/fuzzer.cpp)
#    add_sanitizers(fuzz_libparsers_function_scripts)
#endif()

#############
# tests_old #
#############

file(GLOB_RECURSE OLD_TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests_old/*.cpp")
file(GLOB_RECURSE OLD_TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests_old/*.hpp")



configure_file(tests_old/old_tests_config.hpp.in
               old_tests_config.hpp)

set(CMAKE_INCLUDE_CURRENT_DIR True)

add_executable(bve-old-tests ${OLD_TEST_SOURCES} ${OLD_TEST_HEADERS})
target_include_directories(bve-old-tests PRIVATE src)
target_link_libraries(bve-old-tests PRIVATE bve-core bve-parsers)
target_link_libraries(bve-old-tests PRIVATE Threads::Threads cppfs::cppfs)
target_link_libraries(bve-old-tests PRIVATE options::fast_math)

set_property(TARGET bve-old-tests PROPERTY FOLDER lib/tests)

add_sanitizers(bve-old-tests)
format(bve-old-tests)
cotire(bve-old-tests)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES} ${OLD_TEST_SOURCES} ${OLD_TEST_HEADERS} ${TEST_HEADERS} ${TEST_SOURCES})
