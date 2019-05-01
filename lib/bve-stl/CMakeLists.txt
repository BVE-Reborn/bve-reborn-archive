file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-stl ${SOURCES} ${HEADERS})
target_include_directories(bve-stl PUBLIC include)

finish_bve_target(bve-stl)

target_link_libraries(bve-stl PUBLIC
        eastl::lib
)

set_property(TARGET bve-stl PROPERTY FOLDER src)
