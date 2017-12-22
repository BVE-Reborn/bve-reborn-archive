set (SOURCES src/math.cpp)

lint(FILES ${SOURCES})
format(FILES ${SOURCES})

add_library(core SHARED ${SOURCES})
target_include_directories(core PUBLIC include)