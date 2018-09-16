##############
# libparsers #
##############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

lint(FILES ${SOURCES})
format(FILES ${SOURCES} ${HEADERS})

add_library(parsers SHARED ${SOURCES} ${HEADERS})

target_compile_features(parsers PUBLIC cxx_std_14)

target_include_directories(parsers PUBLIC include)
target_link_libraries(parsers PUBLIC core glm::glm mapbox::variant gsl::gsl tl::optional std::filesystem)

target_include_directories(parsers PRIVATE src)
target_link_libraries(parsers PUBLIC options::fast_math options::iterator_debug_level_1)
target_link_libraries(parsers PRIVATE rapidxml::rapidxml)
add_sanitizers(parsers)
cotire(parsers)

###################
# libparsers_test #
###################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp")

add_library(libparsers_test OBJECT ${TEST_SOURCES})
target_include_directories(libparsers_test PUBLIC ${CMAKE_SOURCE_DIR}/libparsers/src)
target_include_directories(libparsers_test PUBLIC ${CMAKE_SOURCE_DIR}/libparsers/tests)
target_link_libraries(libparsers_test PUBLIC parsers doctest::doctest options::iterator_debug_level_1)

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})
add_sanitizers(libparsers_test)
cotire(libparsers_test)

#if (BVEREBORN_WITH_FUZZING)
#    add_executable(fuzz_libparsers_function_scripts tests/function_scripts/fuzzer.cpp)
#    target_compile_options(fuzz_libparsers_function_scripts PRIVATE -fsanitize=fuzzer)
#    target_link_libraries(fuzz_libparsers_function_scripts PRIVATE parsers options::iterator_debug_level_1 -fsanitize=fuzzer)
#    lint(FILES tests/function_scripts/fuzzer.cpp)
#    format(FILES tests/function_scripts/fuzzer.cpp)
#    add_sanitizers(fuzz_libparsers_function_scripts)
#endif()

#############
# tests_old #
#############

file(GLOB_RECURSE OLD_TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests_old/*.cpp")
file(GLOB_RECURSE OLD_TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests_old/*.hpp")

lint(FILES ${OLD_TEST_SOURCES})
format(FILES ${OLD_TEST_SOURCES} ${OLD_TEST_HEADERS})

if(BVEREBORN_WITH_OLD_TESTS)
    file(COPY tests_old/test_files
         DESTINATION tests_old/)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests_old/old_tests_config.hpp.in
               old_tests_config.hpp)

set(CMAKE_INCLUDE_CURRENT_DIR True)

add_executable(old_tests ${OLD_TEST_SOURCES} ${OLD_TEST_HEADERS})
target_compile_features(old_tests PRIVATE cxx_std_14)
target_include_directories(old_tests PRIVATE src)
target_link_libraries(old_tests PRIVATE core parsers Threads::Threads options::fast_math options::iterator_debug_level_1)
add_sanitizers(old_tests)
cotire(old_tests)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES} ${OLD_TEST_SOURCES} ${OLD_TEST_HEADERS} ${TEST_HEADERS} ${TEST_SOURCES})
