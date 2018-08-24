set_property(DIRECTORY
    PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/doctest/doctest"
    "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_executable(test_main main.cpp)

target_link_libraries(test_main PUBLIC doctest::doctest options::iterator_debug_level_1) # Global tests
target_link_libraries(test_main PUBLIC libparsers_test)

# liblog
target_link_libraries(test_main PUBLIC log)
target_link_libraries(test_main PUBLIC Boost::regex Threads::Threads)

lint(FILES main.cpp)
format(FILES main.cpp)
add_sanitizers(test_main)
cotire(test_main)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES main.cpp)
