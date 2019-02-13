find_program(
	CLANG_TIDY_EXE
	NAMES "clang-tidy"
	      "clang-tidy-7.0"
	      "clang-tidy-6.0"
	      "clang-tidy-5.0"
	      "clang-tidy-4.0"
	      "clang-tidy-3.9"
	      "clang-tidy-3.8"
	      "clang-tidy-3.7"
	      "clang-tidy-3.6"
	      "clang-tidy-3.5"
	DOC "Path to clang-tidy executable")

if(NOT CLANG_TIDY_EXE)
	message(STATUS "run-clang-tidy not found.")
else()
	message(STATUS "run-clang-tidy found: ${CLANG_TIDY_EXE}")
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(THIS_DIR ${CMAKE_CURRENT_LIST_DIR})

set(CLANG_TIDY_MANAGER "${THIS_DIR}/../clang-tidy-manager/clang-tidy-manager.py")

if (NOT FILES_TO_LINT) 
	set(FILES_TO_LINT " " CACHE INTERNAL "FILES_TO_LINT" FORCE)
endif()
if (NOT DIRECTORIES_TO_LINT) 
	set(DIRECTORIES_TO_LINT " " CACHE INTERNAL "DIRECTORIES_TO_LINT" FORCE)
endif()

function(lint) 
	cmake_parse_arguments(LT "" "" "FILES" ${ARGN})

	foreach(file ${LT_FILES})
		set(FILES_TO_LINT ${FILES_TO_LINT} ${CMAKE_CURRENT_SOURCE_DIR}/${file} CACHE INTERNAL "FILES_TO_LINT" FORCE)
	endforeach()
endfunction(lint)

function(add_linted_subdirectory)
	set(DIRECTORIES_TO_LINT ${DIRECTORIES_TO_LINT} "${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}" CACHE INTERNAL "DIRECTORIES_TO_LINT" FORCE)
	add_subdirectory(${ARGN})
endfunction(add_linted_subdirectory)

function(configure_linter)
	list(REMOVE_DUPLICATES FILES_TO_LINT)
	list(REMOVE_DUPLICATES DIRECTORIES_TO_LINT)

	foreach(file ${FILES_TO_LINT})
		set(FILES_TO_LINT_STR "${FILES_TO_LINT_STR} \"${file}\"")
	endforeach()

	foreach(dir ${DIRECTORIES_TO_LINT})
		set(DIR_TO_LINT_STR "${DIR_TO_LINT_STR} ${dir}")
	endforeach()

	add_custom_target(lint ${CLANG_TIDY_MANAGER} -p "${CMAKE_BINARY_DIR}" --path-to-clang-tidy=${CLANG_TIDY_EXE} --folders ${DIRECTORIES_TO_LINT} VERBATIM USES_TERMINAL)	
	add_custom_target(lint-fix ${CLANG_TIDY_MANAGER} -p "${CMAKE_BINARY_DIR}" --path-to-clang-tidy=${CLANG_TIDY_EXE} --folders ${DIRECTORIES_TO_LINT} --fix VERBATIM USES_TERMINAL)	
	
	set(FILES_TO_LINT ${FILES_TO_LINT} CACHE INTERNAL "FILES_TO_LINT" FORCE)
	set(DIRECTORIES_TO_LINT ${DIRECTORIES_TO_LINT} CACHE INTERNAL "DIRECTORIES_TO_LINT" FORCE)
endfunction(configure_linter)
