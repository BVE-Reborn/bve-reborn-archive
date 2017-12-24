set(SOURCES 
            src/csv_object_instruction_generator.cpp
            src/csv_object_instruction_iostream.cpp
			src/csv_object_generator.cpp
            src/csv_parser.cpp
            src/csv_parser_iostream.cpp
            src/utils.cpp
    )

find_package(Boost 1.44 REQUIRED)

lint(FILES ${SOURCES})
format(FILES ${SOURCES})

add_library(parsers SHARED ${SOURCES})
target_include_directories(parsers PUBLIC include)
target_include_directories(parsers PRIVATE src)
target_link_libraries(parsers PRIVATE Boost::boost)
target_link_libraries(parsers PUBLIC core)
target_link_libraries(parsers PUBLIC glm)

set(TEST_SOURCES 
                 tests/main.cpp
                 tests/test_csv.cpp
                 tests/test_csv_obj.cpp
    )

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES})

configure_file(tests/pole_2.csv tests/pole_2.csv COPYONLY)
configure_file(tests/ind4.csv tests/ind4.csv COPYONLY)
configure_file(tests/westonmill_2.b3d tests/westonmill_2.b3d COPYONLY)

add_executable(parsers_test ${TEST_SOURCES})
target_include_directories(parsers_test PRIVATE src)
target_link_libraries(parsers_test parsers Boost::boost)
