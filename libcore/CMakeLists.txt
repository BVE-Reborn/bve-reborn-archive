set (SOURCES 
             src/math.cpp
             )
set (HEADERS 
             include/core/datatypes.hpp
             include/core/math.hpp
             include/core/variant_util.hpp
    )

lint(FILES ${SOURCES})
format(FILES ${SOURCES})

add_library(core SHARED ${SOURCES} ${HEADERS})
target_include_directories(core INTERFACE include)
target_include_directories(core SYSTEM INTERFACE ${CMAKE_SOURCE_DIR}/dependencies/mapbox-variant/include)
target_link_libraries(core INTERFACE glm)
