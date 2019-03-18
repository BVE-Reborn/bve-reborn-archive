##############
# libbve-log #
##############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-log SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-log PUBLIC include)
target_link_libraries(bve-log PUBLIC fmt::fmt bve-util)

finish_bve_target(bve-log)

set_property(TARGET bve-log PROPERTY FOLDER lib)

###################
# libbve-log-test #
###################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_bve_library(bve-log-test OBJECT ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(bve-log-test PUBLIC bve-log bve-util)
target_link_libraries(bve-log-test PUBLIC Threads::Threads)

finish_bve_target(bve-log-test)

set_property(TARGET bve-log-test PROPERTY FOLDER lib/tests)
