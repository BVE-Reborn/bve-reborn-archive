set (SOURCES 
             src/math.cpp
             )
set (HEADERS 
             include/core/datatypes.hpp
             include/core/math.hpp
    )

lint(FILES ${SOURCES})
format(FILES ${SOURCES})

add_library(core SHARED ${SOURCES})
target_include_directories(core INTERFACE include)
target_link_libraries(core INTERFACE glm)