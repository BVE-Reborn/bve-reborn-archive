add_executable(test_main main.cpp)

target_link_libraries(test_main PUBLIC parsers_tests)

target_link_libraries(test_main PUBLIC doctest::doctest options::iterator_debug_level_1)
lint(FILES main.cpp)
format(FILES main.cpp)
add_sanitizers(test_main)
cotire(test_main)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES main.cpp)
