add_library(glm::glm INTERFACE IMPORTED)
set_target_properties(glm::glm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/glm"
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
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/cpp_guidelines_support_library/include"
	INTERFACE_COMPILE_FEATURES cxx_std_17
	INTERFACE_COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION"
)

add_library(tl::optional INTERFACE IMPORTED)
set_target_properties(tl::optional PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/optional"
	INTERFACE_COMPILE_FEATURES cxx_std_17
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/fmt)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake-sanitizers/cmake" ${CMAKE_MODULE_PATH})
if(WIN32)
	target_compile_options(fmt PRIVATE "-D_ITERATOR_DEBUG_LEVEL=1")
else()
	target_compile_options(fmt PRIVATE "-fPIC")
endif()

find_package(Sanitizers)

