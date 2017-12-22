set(SOURCES src/csv_object_parser.cpp
            src/csv_parser.cpp
            src/utils.cpp
    )

find_package(Boost 1.44 REQUIRED)

add_library(parsers SHARED ${SOURCES})
target_include_directories(parsers PUBLIC include)
target_include_directories(parsers PRIVATE src)
target_link_libraries(parsers datatypes Boost::boost)

set(TEST_SOURCES tests/test_csv.cpp)

add_executable(parsers_test ${TEST_SOURCES})
target_include_directories(parsers_test PRIVATE src)
target_link_libraries(parsers_test parsers)
