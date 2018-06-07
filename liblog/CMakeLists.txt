###########
# liblog #
###########

set (SOURCES
             src/log.cpp
    )
set (HEADERS
             include/log.hpp
    )

lint(FILES ${SOURCES})
format(FILES ${HEADERS} ${SOURCES})

add_library(log SHARED ${SOURCES} ${HEADERS})
target_compile_features(log PUBLIC cxx_std_14)
target_include_directories(log PUBLIC include)
target_link_libraries(log PRIVATE options::fast_math options::iterator_debug_level_1
        gsl::gsl
                      # options::best_linker
    )
target_link_libraries(log PUBLIC fmt::fmt)
add_sanitizers(log)
cotire(log)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})
