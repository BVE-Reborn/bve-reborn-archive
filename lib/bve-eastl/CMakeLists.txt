file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-eastl ${SOURCES} ${HEADERS})
target_include_directories(bve-eastl PUBLIC include)

finish_bve_target(bve-eastl)

target_link_libraries(bve-eastl PUBLIC
        eastl::lib
)

set_property(TARGET bve-eastl PROPERTY FOLDER src)
