file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

set_property(DIRECTORY
    PROPERTY COTIRE_PREFIX_HEADER_IGNORE_PATH
    "${CMAKE_SOURCE_DIR}/dependencies/doctest/doctest"
    "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_bve_executable(tests ${SOURCES} ${HEADERS})

target_link_libraries(tests PUBLIC bve-core-test)
target_link_libraries(tests PUBLIC bve-parsers-test)
target_link_libraries(tests PUBLIC bve-util-test)
target_link_libraries(tests PUBLIC foundational::foundational)

finish_bve_target(tests)

set_property(TARGET tests PROPERTY FOLDER src)
