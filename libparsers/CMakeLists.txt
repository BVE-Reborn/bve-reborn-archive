set(SOURCES 
            src/b3d_csv_object/executor.cpp
            src/b3d_csv_object/instruction_generator.cpp
            src/b3d_csv_object/instruction_iostream.cpp
            src/b3d_csv_object/parse.cpp
            src/core_csv/iostream.cpp
            src/core_csv/parser.cpp
            src/utils.cpp
    )

set(HEADERS
            include/parsers/parsers.hpp
            src/b3d_csv_object.hpp
            src/csv.hpp
)

find_package(Boost 1.44 REQUIRED)

lint(FILES ${SOURCES})
format(FILES ${SOURCES} ${HEADERS})

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

set(TEST_HEADERS
                 tests/tests.hpp
    )

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})

configure_file(tests/pole_2.csv tests/pole_2.csv COPYONLY)
configure_file(tests/ind4.csv tests/ind4.csv COPYONLY)
configure_file(tests/westonmill_2.b3d tests/westonmill_2.b3d COPYONLY)

add_executable(parsers_test ${TEST_SOURCES})
target_include_directories(parsers_test PRIVATE src)
target_link_libraries(parsers_test parsers Boost::boost)
