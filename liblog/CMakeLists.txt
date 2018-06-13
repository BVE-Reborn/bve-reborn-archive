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



set(TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/log_debug.cpp
        )

set(TEST_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/log_test.hpp
        )

set_property(GLOBAL APPEND PROPERTY OPENBVE2_TEST_FILE_LIST "${TEST_SOURCES};${TEST_HEADERS}")
lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${TEST_HEADERS} ${TEST_SOURCES})
