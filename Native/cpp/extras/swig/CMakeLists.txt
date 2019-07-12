file(GLOB_RECURSE HEADER_INTERFACES LIST_DIRECTORIES false CONFIGURE_DEPENDS "include/*.i" "src/*.i")
set(SOURCE_INTERFACES ${CMAKE_CURRENT_LIST_DIR}/src/bve-cs.i)

foreach(file ${SOURCE_INTERFACES})
    set_property(SOURCE "${file}" PROPERTY CPLUSPLUS ON)
    set_property(SOURCE "${file}" PROPERTY COMPILE_OPTIONS "-Wextra;-dllimport;bve-cs;-namespace;Native" APPEND)
    set_property(SOURCE "${file}" PROPERTY DEPENDS ${HEADER_INTERFACES} APPEND)
endforeach()

swig_add_library(bve-cs 
                 TYPE SHARED 
                 LANGUAGE csharp 
                 SOURCES ${SOURCE_INTERFACES}
                 OUTPUT_DIR ${CMAKE_BINARY_DIR}/bindings/csharp
                 OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/csharp
)
if(RUST)
    set(RUST_INCLUDE_DIRS
                     ${RUST_HEADER_PATH}
                     ${RUST_SWIG_PATH}
    )
    set_property(TARGET bve-cs 
                 PROPERTY 
                   SWIG_GENERATED_INCLUDE_DIRECTORIES 
                     ${RUST_INCLUDE_DIRS}
    )
endif()
swig_link_libraries(bve-cs bve-core bve-eastl bve-parsers bve-util ${RUST_LINK_LIBRARIES})
set_property(TARGET bve-cs 
             PROPERTY 
               SWIG_INCLUDE_DIRECTORIES 
                 ${CMAKE_CURRENT_SOURCE_DIR}/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-core/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-eastl/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-parsers/include
                 ${CMAKE_SOURCE_DIR}/libs/bve-util/include
                 ${RUST_INCLUDE_DIRS}
)

set_property(TARGET bve-cs PROPERTY FOLDER lib/bindings/csharp)
