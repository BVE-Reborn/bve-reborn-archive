find_package(Threads REQUIRED)

find_package(unofficial-abseil CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(cppfs CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set_property(TARGET glm APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS GLM_ENABLE_EXPERIMENTAL)

find_package(VTune)
