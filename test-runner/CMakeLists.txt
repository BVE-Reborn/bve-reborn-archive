set_property(DIRECTORY
    PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/doctest/doctest"
    "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_executable(tests main.cpp)

target_link_libraries(tests PUBLIC doctest::doctest) # Global tests
target_link_libraries(tests PUBLIC bve-parsers-test)
target_link_libraries(tests PUBLIC bve-parsers-test)

lint(FILES main.cpp)
format(FILES main.cpp)
add_sanitizers(tests)
cotire(tests)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES main.cpp)
