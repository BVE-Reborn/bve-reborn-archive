set_property(DIRECTORY
    PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/doctest/doctest"
    "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_bve_executable(tests src/main.cpp)

target_link_libraries(tests PUBLIC bve-core-test)
target_link_libraries(tests PUBLIC bve-parsers-test)
target_link_libraries(tests PUBLIC bve-util-test)

finish_bve_target(tests)

set_property(TARGET tests PROPERTY FOLDER src)
