set(SOURCES src/csv_object_parser.cpp
	        src/utils.cpp
	)

find_package(Boost REQUIRED)

add_library(parsers SHARED ${SOURCES})
target_include_directories(parsers PUBLIC include)
target_include_directories(parsers PRIVATE src)
target_link_libraries(parsers datatypes)