###############
# libbve-core #
###############

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.hpp")

add_bve_library(bve-core SHARED ${SOURCES} ${HEADERS})
target_include_directories(bve-core PUBLIC include)
target_link_libraries(bve-core PUBLIC bve-util cppfs::cppfs glm gsl::gsl foundational::foundational)

if(BVE_VTUNE)
    target_link_libraries(bve-core PUBLIC vtune::vtune)
endif()

finish_bve_target(bve-core)

set_property(TARGET bve-core PROPERTY FOLDER lib)

####################
# libbve-core-test #
####################

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.cpp")
file(GLOB_RECURSE TEST_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "tests/*.hpp")

add_bve_library(bve-core-test OBJECT ${TEST_SOURCES} ${TEST_HEADERS})
target_link_libraries(bve-core-test PUBLIC bve-core bve-util)
target_link_libraries(bve-core-test PUBLIC Threads::Threads)

finish_bve_target(bve-core-test)

set_property(TARGET bve-core-test PROPERTY FOLDER lib/tests)

##########
## swig ##
##########
if(SWIG_FOUND)
	file(GLOB_RECURSE INTERFACES LIST_DIRECTORIES false CONFIGURE_DEPENDS "extras/*.i")

	foreach(file ${INTERFACES})
		set_property(SOURCE "${file}" PROPERTY CPLUSPLUS ON)
		set_property(SOURCE "${file}" PROPERTY CMAKE_SWIG_FLAGS "-Wall;-Wextra;-py3" APPEND)
	endforeach()

	swig_add_library(bve_core_python TYPE SHARED LANGUAGE python SOURCES ${INTERFACES} OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/extras/python)
	swig_link_libraries(bve_core_python bve-core Python3::Python)
	set_property(TARGET bve_core_python PROPERTY SWIG_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)

	set_property(TARGET bve_core_python PROPERTY FOLDER lib/bindings/python)
endif()
