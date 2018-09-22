###############
# ibbve-core #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

lint(FILES ${SOURCES})
format(FILES ${HEADERS} ${SOURCES})

add_library(bve-core SHARED ${SOURCES} ${HEADERS})
target_compile_features(bve-core PUBLIC cxx_std_14)
target_include_directories(bve-core PUBLIC include)
target_link_libraries(bve-core PUBLIC glm::glm mapbox::variant gsl::gsl absl::optional)
target_link_libraries(bve-core PRIVATE options::fast_math)
add_sanitizers(bve-core)
cotire(bve-core)

set_property(TARGET bve-core PROPERTY FOLDER lib)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})
