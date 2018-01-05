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
            src/csv_rw_route/instruction_generator.cpp
            src/csv_rw_route/instruction_iostream.cpp
            src/csv_rw_route/command_spliter.cpp
            src/csv_rw_route/preprocessor.cpp
            src/csv_rw_route/process_includes.cpp
            src/function_scripts/instruction_builder.cpp
            src/function_scripts/instruction_iostream.cpp
            src/function_scripts/lexer.cpp
            src/function_scripts/lexer_iostream.cpp
            src/function_scripts/parsefunc.cpp
            src/function_scripts/parser.cpp
            src/function_scripts/parser_iostream.cpp
            src/utils.cpp
    )

set(HEADERS
            include/parsers/animated.hpp
            include/parsers/b3d_csv.hpp
            include/parsers/dependencies.hpp
            include/parsers/errors.hpp
            include/parsers/function_scripts.hpp
            include/parsers/parsers.hpp
            src/b3d_csv_object.hpp
            src/csv.hpp
            src/csv_rw_route.hpp
            src/csv_rw_route/instructions.hpp
            src/function_scripts.hpp
            src/ini.hpp
)

find_package(Boost 1.44 REQUIRED)

lint(FILES ${SOURCES})
format(FILES ${SOURCES} ${HEADERS})

add_library(parsers SHARED ${SOURCES} ${HEADERS})
target_include_directories(parsers PUBLIC include)
target_include_directories(parsers PRIVATE src)
target_include_directories(parsers SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/mapbox-variant/include)
target_link_libraries(parsers PRIVATE Boost::boost)
target_link_libraries(parsers PUBLIC core)
target_link_libraries(parsers PUBLIC glm)

set(TEST_SOURCES 
                 tests/main.cpp
                 tests/test_animated.cpp
                 tests/test_csv.cpp
                 tests/test_csv_obj.cpp
                 tests/test_csv_route.cpp
                 tests/test_fs.cpp
    )

set(TEST_HEADERS
                 tests/tests.hpp
    )

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})

file(COPY tests/pole_2.csv
          tests/ind4.csv
          tests/westonmill_2.b3d
          tests/81xx_2.b3d
          tests/r160.animated
          tests/plymouth
     DESTINATION tests/)

add_executable(parsers_test ${TEST_SOURCES})
target_include_directories(parsers_test PRIVATE src)
target_link_libraries(parsers_test parsers Boost::boost)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(parsers_test -lstdc++fs)
endif()
