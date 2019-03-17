find_package(unofficial-abseil CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
find_package(cppfs CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

find_package(Sanitizers REQUIRED)

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
