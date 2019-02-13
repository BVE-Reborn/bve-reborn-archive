include(CMakeParseArguments)

find_program(
	CLANG_FORMAT_EXE
	NAMES "clang-format"
	      "clang-format-6.0"
	      "clang-format-5.0"
	      "clang-format-4.0"
	      "clang-format-3.9"
	      "clang-format-3.8"
	      "clang-format-3.7"
	      "clang-format-3.6"
	      "clang-format-3.5"
	DOC "Path to clang-format executable")
if(NOT CLANG_FORMAT_EXE)
	message(STATUS "clang-format not found.")
else()
	message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
endif()

if(CLANG_FORMAT_EXE)
	add_custom_target(format)
endif()

function(format)
	if (CLANG_FORMAT_EXE AND UNIX)
		cmake_parse_arguments(CF "" "" "FILES" ${ARGN})

		foreach(file ${CF_FILES})
			string(FIND ${file} "${CMAKE_SOURCE_DIR}" FOUND_SOURCE_DIR_LOC)
			if (${FOUND_SOURCE_DIR_LOC} EQUAL -1)
				set(CURRENT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${file})
			else()
				set(CURRENT_FILE "${file}")
			endif()
#			message(STATUS "${CURRENT_FILE}")
			string(REPLACE  "/" "__" CURRENT_SUBTARGET ${CURRENT_FILE})
			set(CF_SUBTARGETS ${CF_SUBTARGETS} format_subtarget_${CURRENT_SUBTARGET})
			add_custom_target(format_subtarget_${CURRENT_SUBTARGET} 
			                  COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${CURRENT_FILE})
		endforeach()
		add_dependencies(format ${CF_SUBTARGETS})
	endif()
endfunction(format)
