###########
# libcore #
###########

set (SOURCES 
             src/math.cpp
             )
set (HEADERS 
             include/core/datatypes.hpp
             include/core/math.hpp
             include/core/variant_util.hpp
    )

lint(FILES ${SOURCES})
format(FILES ${SOURCES})

add_library(core SHARED ${SOURCES} ${HEADERS})
target_compile_features(core PUBLIC cxx_std_14)
target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC glm::glm mapbox::variant)
target_link_libraries(core PRIVATE options::fast_math options::iterator_debug_level_1 options::best_linker)
add_sanitizers(core)
cotire(core)
