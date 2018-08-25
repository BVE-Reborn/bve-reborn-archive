###########
# liblog #
###########

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

lint(FILES ${SOURCES})
format(FILES ${HEADERS} ${SOURCES})

add_library(log SHARED ${SOURCES} ${HEADERS})
target_compile_features(log PUBLIC cxx_std_14)
target_include_directories(log PUBLIC include)
target_link_libraries(log PRIVATE options::fast_math options::iterator_debug_level_1 gsl::gsl
                      # options::best_linker
    )
target_link_libraries(log PUBLIC fmt::fmt)
add_sanitizers(log)
cotire(log)

###############
# liblog_test #
###############

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp")

add_library(liblog_test OBJECT ${TEST_SOURCES} ${TEST_HEADERS})
target_include_directories(liblog_test PUBLIC ../libparsers/src)
target_link_libraries(liblog_test PUBLIC log core)
target_link_libraries(liblog_test PUBLIC Boost::regex Threads::Threads gsl::gsl doctest::doctest
                                         options::iterator_debug_level_1 Boost::filesystem)
add_sanitizers(liblog_test)
cotire(liblog_test)

lint(FILES ${TEST_SOURCES})
format(FILES ${TEST_SOURCES} ${TEST_HEADERS})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${TEST_HEADERS} ${TEST_SOURCES})
