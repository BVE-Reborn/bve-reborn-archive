###########
# libcore #
###########

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

lint(FILES ${SOURCES})
format(FILES ${HEADERS} ${SOURCES})

add_library(core SHARED ${SOURCES} ${HEADERS})
target_compile_features(core PUBLIC cxx_std_14)
target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC glm::glm mapbox::variant gsl::gsl)
target_link_libraries(core PRIVATE options::fast_math options::iterator_debug_level_1 
                                   # options::best_linker
                                   )
add_sanitizers(core)
cotire(core)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})
