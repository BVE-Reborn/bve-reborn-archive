include(CMakeParseArguments)

find_package(PythonInterp 3.5 REQUIRED)

function(add_python_program) 
	cmake_parse_arguments(PT "" "NAME" "FILES;TESTS;DATA" ${ARGN})

	set(ALL_FILES ${PT_FILES} ${PT_TESTS})

	foreach (file ${ALL_FILES})
		get_filename_component(OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/${file} DIRECTORY)
		get_filename_component(OUTPUT_LOG_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/log/${file} DIRECTORY)

		set (PY_COMPILE_FLAGS ${PYTHON_EXECUTABLE} -B -m py_compile "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
		add_custom_command(OUTPUT log/${file}.compile
			               COMMAND mkdir -p ${OUTPUT_LOG_FOLDER}
			               COMMAND bash ARGS ${CMAKE_CURRENT_LIST_DIR}/../runnable/output_harness.bash "${CMAKE_CURRENT_BINARY_DIR}/log/${file}.compile" '${PEP8_COMPILE_FLAGS}'
			               DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
		
		set (PEP8_COMPILE_FLAGS ${PYTHON_EXECUTABLE} -B -m pep8 --show-source --max-line-length=120 --ignore=E402 ${CMAKE_CURRENT_SOURCE_DIR}/${file})
		add_custom_command(OUTPUT log/${file}.pep8
		                   COMMAND mkdir -p ${OUTPUT_LOG_FOLDER}
		                   COMMAND bash ARGS ${CMAKE_CURRENT_LIST_DIR}/../runnable/output_harness.bash "${CMAKE_CURRENT_BINARY_DIR}/log/${file}.pep8" '${PEP8_COMPILE_FLAGS}'
		                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file} log/${file}.compile)
		
		add_custom_command(OUTPUT ${file}
		                   COMMAND mkdir -p ${OUTPUT_FOLDER}
		                   COMMAND cp ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file}
		                   COMMAND chmod ARGS +x ${CMAKE_CURRENT_BINARY_DIR}/${file}
		                   DEPENDS log/${file}.pep8 log/${file}.compile)
		set(PT_DEPEND ${PT_DEPEND} "${file}")
	endforeach()

	foreach (data ${PT_DATA})
		get_filename_component(OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/${data} DIRECTORY)
		add_custom_command(OUTPUT ${data}
		                   COMMAND mkdir -p ${OUTPUT_FOLDER}
		                   COMMAND cp ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${data} ${CMAKE_CURRENT_BINARY_DIR}/${data}
		                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${data})
		set(PT_DEPEND ${PT_DEPEND} "${data}")
	endforeach()

	foreach (test ${PT_TESTS})
		set(NAME ${PT_NAME}/${test})
		add_test(${NAME} ${PYTHON_EXECUTABLE} ${test} -v)
	endforeach()

	add_custom_target(${PT_NAME} ALL DEPENDS ${PT_DEPEND})
endfunction(add_python_program)
