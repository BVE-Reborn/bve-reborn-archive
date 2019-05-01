file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-eastl ${SOURCES} ${HEADERS})
target_include_directories(bve-eastl PUBLIC include)

finish_bve_target(bve-eastl)

target_link_libraries(bve-eastl PUBLIC
        eastl::lib
)
target_compile_definitions(bve-eastl PUBLIC $<$<CONFIG:Debug>:EASTL_ASSERT_ENABLED> EASTL_EXCEPTIONS_ENABLED=0 EASTLAllocatorType=::bve::util::AllocatorHandle EASTLAllocatorDefault=::bve::util::default_allocator)
if(OS_WINDOWS)
    target_compile_options(bve-eastl PUBLIC /FI:${CMAKE_SOURCE_DIR}/lib/bve-util/allocators.hpp)
else()
    target_compile_options(bve-eastl PUBLIC -include ${CMAKE_SOURCE_DIR}/lib/bve-eastl/include/EASTL/allocator_fwd.hpp)
endif()
set_target_properties(bve-eastl PROPERTIES POSITION_INDEPENDENT_CODE True)

set_property(TARGET bve-eastl PROPERTY FOLDER src)
