file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_executable(stress-test ${SOURCES} ${HEADERS})

finish_bve_target(stress-test)

target_link_libraries(stress-test
                      PRIVATE CLI11::CLI11
                              foundational::foundational
                              cppfs::cppfs
                              bve-eastl
                              bve-core
                              bve-parsers
                              bve-util)

set_property(TARGET stress-test PROPERTY FOLDER src)
