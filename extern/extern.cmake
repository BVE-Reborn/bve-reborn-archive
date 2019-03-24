find_package(Threads REQUIRED)

# NOVA
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
find_package(minitrace CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(miniz CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)
find_package(spirv_cross_core CONFIG REQUIRED)
find_package(spirv_cross_glsl CONFIG REQUIRED)
find_package(spirv_cross_cpp CONFIG REQUIRED)
find_package(spirv_cross_hlsl CONFIG REQUIRED)
find_package(spirv_cross_msl CONFIG REQUIRED)
find_package(spirv_cross_reflect CONFIG REQUIRED)
find_package(spirv_cross_util CONFIG REQUIRED)
find_package(SPIRV-Tools CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

find_package(foundational CONFIG REQUIRED)
find_package(nova-renderer CONFIG REQUIRED)

# BVE
find_package(cppfs CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(unofficial-abseil CONFIG REQUIRED)

set_property(TARGET glm APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS GLM_ENABLE_EXPERIMENTAL)

add_library(gsl::gsl INTERFACE IMPORTED)
set_property(TARGET gsl::gsl APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS GSL_THROW_ON_CONTRACT_VIOLATION)

if(BVE_VTUNE)
	find_package(VTune)
endif()
