file(GLOB_RECURSE HEADER_INTERFACES LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.i" "src/*.i")
set(SOURCE_INTERFACES ${CMAKE_CURRENT_LIST_DIR}/src/bve-cs.i)

foreach(file ${SOURCE_INTERFACES})
    set_property(SOURCE "${file}" PROPERTY CPLUSPLUS ON)
    set_property(SOURCE "${file}" PROPERTY COMPILE_OPTIONS "-Wall;-Wextra;-dllimport;bve-cs;-namespace;Native" APPEND)
    set_property(SOURCE "${file}" PROPERTY DEPENDS ${HEADER_INTERFACES} APPEND)
endforeach()

swig_add_library(bve-cs 
                 TYPE SHARED 
                 LANGUAGE csharp 
                 SOURCES ${SOURCE_INTERFACES}
                 OUTPUT_DIR ${CMAKE_BINARY_DIR}/bindings/csharp
                 OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/csharp
)
swig_link_libraries(bve-cs bve-core bve-eastl bve-parsers bve-util)
set_property(TARGET bve-cs 
             PROPERTY 
               SWIG_INCLUDE_DIRECTORIES 
                 ${CMAKE_CURRENT_SOURCE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-core/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-eastl/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-parsers/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-util/include
)

set_property(TARGET bve-cs PROPERTY FOLDER lib/bindings/csharp)
