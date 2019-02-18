set_property(DIRECTORY
    PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/doctest/doctest"
    "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_executable(tests src/main.cpp)

target_link_libraries(tests PUBLIC doctest::doctest) # Global tests
target_link_libraries(tests PUBLIC bve-core-test)
target_link_libraries(tests PUBLIC bve-log-test)
target_link_libraries(tests PUBLIC bve-parsers-test)
target_link_libraries(tests PUBLIC bve-util-test)


set_property(TARGET tests PROPERTY FOLDER src)

add_sanitizers(tests)
format(tests)
cotire(tests)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES src/main.cpp)
