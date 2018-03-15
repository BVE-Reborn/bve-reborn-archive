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
target_link_libraries(parsers PUBLIC core glm::glm mapbox::variant)

target_include_directories(parsers PRIVATE src)
target_link_libraries(parsers PRIVATE Boost::boost
                                      rapidxml::rapidxml
                                      options::fast_math options::iterator_debug_level_1
                                      # options::best_linker
                                      
                                      )
add_sanitizers(parsers)
cotire(parsers)

# message(STATUS "${flaggers} ${includes} ${sysincludes} ${defines}")
# message(STATUS "${_sourceFiles}")

################
# parsers_test #
################

set(TEST_SOURCES 
                 tests/main.cpp
                 tests/test_animated.cpp
                 tests/test_csv.cpp
                 tests/test_csv_obj.cpp
                 tests/test_csv_route.cpp
                 tests/test_fs.cpp
                 tests/test_math.cpp
    )

set(TEST_HEADERS
                 tests/tests.hpp
    )

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})

if(NOT OPENBVE2_ON_TRAVIS)
    file(COPY tests/test_files
         DESTINATION tests/)
endif()

add_executable(parsers_test ${TEST_SOURCES})
add_sanitizers(parsers_test)
target_compile_features(parsers_test PRIVATE cxx_std_14)
target_include_directories(parsers_test PRIVATE src)
target_link_libraries(parsers_test PRIVATE core parsers
                                           Boost::boost Boost::filesystem
                                           Threads::Threads
                                           options::fast_math options::iterator_debug_level_1 
                                           # options::best_linker
                                           )
add_sanitizers(parsers_test)
cotire(parsers_test)
