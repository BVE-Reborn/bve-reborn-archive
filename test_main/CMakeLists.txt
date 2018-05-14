set_property(DIRECTORY
    PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/doctest/doctest"
    "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

get_property(all_test_files GLOBAL PROPERTY OPENBVE2_TEST_FILE_LIST)

add_executable(test_main main.cpp ${all_test_files})

target_link_libraries(test_main PUBLIC doctest::doctest options::iterator_debug_level_1) # Global tests

# libparsers
target_include_directories(test_main PUBLIC ${CMAKE_SOURCE_DIR}/libparsers/src)
target_include_directories(test_main PUBLIC ${CMAKE_SOURCE_DIR}/libparsers/tests)
target_link_libraries(test_main PUBLIC parsers)

lint(FILES main.cpp)
format(FILES main.cpp)
add_sanitizers(test_main)
cotire(test_main)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES main.cpp)
