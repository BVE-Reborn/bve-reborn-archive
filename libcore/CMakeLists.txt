###########
# libcore #
###########

set (SOURCES 
             src/math.cpp
             )
set (HEADERS 
             include/core/datatypes.hpp
             include/core/math.hpp
             include/core/gsl_string_span_iostream.hpp
             include/core/variant_util.hpp
    )

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
