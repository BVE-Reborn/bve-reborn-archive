add_library(nova::glm INTERFACE IMPORTED)
set_target_properties(nova::glm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/nova/3rdparty/glm"
	INTERFACE_COMPILE_FEATURES cxx_std_17
	INTERFACE_COMPILE_DEFINITIONS "GLM_ENABLE_EXPERIMENTAL"
)

add_library(mapbox::variant INTERFACE IMPORTED)
set_target_properties(mapbox::variant PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/mapbox-variant/include"
	INTERFACE_COMPILE_FEATURES cxx_std_17
)

add_library(rapidxml::rapidxml INTERFACE IMPORTED)
set_target_properties(rapidxml::rapidxml PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/rapidxml_ns"
	INTERFACE_COMPILE_FEATURES cxx_std_17
)

add_library(doctest::doctest INTERFACE IMPORTED)
set_target_properties(doctest::doctest PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/doctest/doctest"
	INTERFACE_COMPILE_FEATURES cxx_std_17
)

add_library(gsl::gsl INTERFACE IMPORTED)
set_target_properties(gsl::gsl PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/gsl/include"
	INTERFACE_COMPILE_FEATURES cxx_std_17
	INTERFACE_COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION"
)

function(make_target_includes_system target)
	get_target_property(INTERFACE_INCLUDES ${target} INTERFACE_INCLUDE_DIRECTORIES)
	get_target_property(PUBLIC_INCLUDES ${target} PUBLIC_INCLUDE_DIRECTORIES)
	if(INTERFACE_INCLUDES)
		target_include_directories(${target} SYSTEM BEFORE INTERFACE ${INTERFACE_INCLUDES})
	endif()
	if(PUBLIC_INCLUDES)
		target_include_directories(${target} SYSTEM BEFORE PUBLIC ${PUBLIC_INCLUDES})
	endif()
endfunction()

set(BUILD_TESTING False CACHE BOOL "Abseil Testing")
set(ABSL_IDE_FOLDER extern/abseil-cpp CACHE STRING "Abseil IDE location")
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/abseil-cpp EXCLUDE_FROM_ALL)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cppfs)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake-sanitizers/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/fmt)
if(WIN32)
	target_compile_options(fmt PRIVATE "-D_ITERATOR_DEBUG_LEVEL=1")
else()
	target_compile_options(fmt PRIVATE "-fPIC")
endif()

make_target_includes_system(fmt)

find_package(Sanitizers)

set_property(TARGET cppfs PROPERTY FOLDER extern)
set_property(TARGET fmt PROPERTY FOLDER extern)
