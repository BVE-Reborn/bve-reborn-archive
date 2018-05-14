##############
# libparsers #
##############

set(SOURCES 
            src/animated_object/parser.cpp
            src/b3d_csv_object/executor.cpp
            src/b3d_csv_object/instruction_generator.cpp
            src/b3d_csv_object/instruction_iostream.cpp
            src/b3d_csv_object/parse.cpp
            src/core_csv/iostream.cpp
            src/core_csv/parser.cpp
            src/core_ini/iostream.cpp
            src/core_ini/parser.cpp
            src/csv_rw_route/command_spliter.cpp
            src/csv_rw_route/executor_pass1.cpp
            src/csv_rw_route/executor_pass2.cpp
            src/csv_rw_route/executor_pass3.cpp
            src/csv_rw_route/executor_pass3/cycle.cpp
            src/csv_rw_route/executor_pass3/finalize.cpp
            src/csv_rw_route/executor_pass3/options.cpp
            src/csv_rw_route/executor_pass3/route.cpp
            src/csv_rw_route/executor_pass3/signals.cpp
            src/csv_rw_route/executor_pass3/structure.cpp
            src/csv_rw_route/executor_pass3/texture.cpp
            src/csv_rw_route/executor_pass3/track_misc.cpp
            src/csv_rw_route/executor_pass3/track_objects.cpp
            src/csv_rw_route/executor_pass3/track_rails.cpp
            src/csv_rw_route/executor_pass3/track_safety.cpp
            src/csv_rw_route/executor_pass3/track_signals_limits.cpp
            src/csv_rw_route/executor_pass3/track_stations.cpp
            src/csv_rw_route/executor_pass3/train.cpp
            src/csv_rw_route/executor_pass3/util.cpp
            src/csv_rw_route/instruction_generation/cycles.cpp
            src/csv_rw_route/instruction_generation/location_statement.cpp
            src/csv_rw_route/instruction_generation/options.cpp
            src/csv_rw_route/instruction_generation/route.cpp
            src/csv_rw_route/instruction_generation/signals.cpp
            src/csv_rw_route/instruction_generation/string_instruction_mapping.cpp
            src/csv_rw_route/instruction_generation/structure.cpp
            src/csv_rw_route/instruction_generation/texture.cpp
            src/csv_rw_route/instruction_generation/track_geometry.cpp
            src/csv_rw_route/instruction_generation/track_misc.cpp
            src/csv_rw_route/instruction_generation/track_objects.cpp
            src/csv_rw_route/instruction_generation/track_rails.cpp
            src/csv_rw_route/instruction_generation/track_safety.cpp
            src/csv_rw_route/instruction_generation/track_signals_limits.cpp
            src/csv_rw_route/instruction_generation/track_stations.cpp
            src/csv_rw_route/instruction_generation/train.cpp
            src/csv_rw_route/instruction_generator.cpp
            src/csv_rw_route/instruction_iostream.cpp
            src/csv_rw_route/preprocessor.cpp
            src/csv_rw_route/process_includes.cpp
            src/errors/add_error.cpp
            src/errors/iostream.cpp
            src/function_scripts/instruction_builder.cpp
            src/function_scripts/instruction_iostream.cpp
            src/function_scripts/lexer.cpp
            src/function_scripts/lexer_iostream.cpp
            src/function_scripts/parsefunc.cpp
            src/function_scripts/parser.cpp
            src/function_scripts/parser_iostream.cpp
            src/utils.cpp
            src/xml/dynamic_background.cpp
            src/xml/dynamic_lighting.cpp
            src/xml/route_marker.cpp
    )

set(HEADERS
            include/parsers/animated.hpp
            include/parsers/b3d_csv.hpp
            include/parsers/dependencies.hpp
            include/parsers/errors.hpp
            include/parsers/find_relative_file.hpp
            include/parsers/function_scripts.hpp
            include/parsers/parsers.hpp
            include/parsers/xml/dynamic_background.hpp
            include/parsers/xml/dynamic_lighting.hpp
            include/parsers/xml/route_marker.hpp
            src/b3d_csv_object.hpp
            src/csv.hpp
            src/csv_rw_route.hpp
            src/csv_rw_route/executor_pass3/executor_pass3.hpp
            src/csv_rw_route/instruction_generation/instruction_generator.hpp
            src/csv_rw_route/instructions.hpp
            src/csv_rw_route/route_structure.hpp
            src/function_scripts.hpp
            src/ini.hpp
)

lint(FILES ${SOURCES})
format(FILES ${SOURCES} ${HEADERS})

add_library(parsers SHARED ${SOURCES} ${HEADERS})

target_compile_features(parsers PUBLIC cxx_std_14)

target_include_directories(parsers PUBLIC include)
target_link_libraries(parsers PUBLIC core glm::glm mapbox::variant gsl::gsl)

target_include_directories(parsers PRIVATE src)
target_link_libraries(parsers PUBLIC Boost::boost Boost::regex
                                     options::fast_math options::iterator_debug_level_1)
target_link_libraries(parsers PRIVATE rapidxml::rapidxml
                                      # options::best_linker
                                      )
add_sanitizers(parsers)
cotire(parsers)

###################
# test libparsers #
###################

set(TEST_SOURCES
                 tests/animated_object/parser.cpp
                 tests/b3d_csv_object/command_parsing.cpp
                 tests/b3d_csv_object/command_execution.cpp
                 tests/core_csv/parse.cpp
                 tests/core_csv/iostream.cpp
                 tests/core_ini/parse.cpp
                 tests/core_ini/iostream.cpp
                 tests/errors/add_error.cpp
                 tests/errors/iostream.cpp
                 tests/function_scripts/errors.cpp
                 tests/function_scripts/functions.cpp
                 tests/function_scripts/iostream_lexer.cpp
                 tests/function_scripts/iostream_tree.cpp
                 tests/function_scripts/iostream_instructions.cpp
                 tests/function_scripts/lexer.cpp
                 tests/function_scripts/operators.cpp
                 tests/function_scripts/order_of_operations.cpp
                 tests/function_scripts/variables.cpp
                 tests/function_scripts/variables_indexed.cpp
                 tests/util/load_from_file_utf8_bom.cpp
                 tests/util/strip_text.cpp
                 tests/util/remove_comments.cpp
                 tests/util/match_against_lower.cpp
                 tests/util/parse_color.cpp
                 tests/util/parse_loose_integer.cpp
                 tests/util/parse_loose_float.cpp
                 tests/util/parse_time.cpp
                 tests/util/split_text.cpp
        )

set(TEST_HEADERS
                 tests/test_macro_util.hpp
    )

add_library(parsers_tests SHARED ${TEST_SOURCES} ${TEST_HEADERS})
target_include_directories(parsers_tests PRIVATE tests src)
target_link_libraries(parsers_tests PRIVATE parsers doctest::doctest Boost::filesystem
                                            options::iterator_debug_level_1)
lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})
add_sanitizers(parsers_tests)
cotire(parsers_tests)

if (OPENBVE2_WITH_FUZZING)
    add_executable(fuzz_libparsers_function_scripts tests/function_scripts/fuzzer.cpp)
    target_compile_options(fuzz_libparsers_function_scripts PRIVATE -fsanitize=fuzzer)
    target_link_libraries(fuzz_libparsers_function_scripts PRIVATE parsers options::iterator_debug_level_1 -fsanitize=fuzzer)
    lint(FILES tests/function_scripts/fuzzer.cpp)
    format(FILES tests/function_scripts/fuzzer.cpp)
    add_sanitizers(fuzz_libparsers_function_scripts)
endif()

#######################
# simple_parsers_test #
#######################

set(SIMPLE_TEST_SOURCES
                 tests/simple_tests/main.cpp
                 tests/simple_tests/test_animated.cpp
                 tests/simple_tests/test_csv.cpp
                 tests/simple_tests/test_csv_obj.cpp
                 tests/simple_tests/test_csv_route.cpp
                 tests/simple_tests/test_fs.cpp
                 tests/simple_tests/test_math.cpp
    )

set(SIMPLE_TEST_HEADERS
                 tests/simple_tests/tests.hpp
    )

lint(FILES ${SIMPLE_TEST_SOURCES})
format(FILES ${SIMPLE_TEST_SOURCES} ${SIMPLE_TEST_HEADERS})

if(NOT OPENBVE2_ON_TRAVIS)
    file(COPY tests/simple_tests/test_files
         DESTINATION tests/)
endif()

add_executable(simple_parsers_test ${SIMPLE_TEST_SOURCES} ${SIMPLE_TEST_HEADERS})
target_compile_features(simple_parsers_test PRIVATE cxx_std_14)
target_include_directories(simple_parsers_test PRIVATE src)
target_link_libraries(simple_parsers_test PRIVATE core parsers
                                           Boost::boost Boost::filesystem
                                           Threads::Threads
                                           options::fast_math options::iterator_debug_level_1
                                           # options::best_linker
                                           )
add_sanitizers(simple_parsers_test)
cotire(simple_parsers_test)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES} ${SIMPLE_TEST_SOURCES} ${SIMPLE_TEST_HEADERS} ${TEST_HEADERS} ${TEST_SOURCES})
