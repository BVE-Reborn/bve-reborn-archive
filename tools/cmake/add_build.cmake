function(add_bve_executable _target)
	set(_args ${ARGN})

	add_executable(${_target} ${_args})
	
	target_add_sanitizers(${_target})
	target_add_warnings(${_target})
	target_bve_cotire(${_target})
	target_bve_coverage(${_target})
	target_format(${_target})

	list(REMOVE_ITEM _args STATIC SHARED OBJECT WIN32 PUBLIC PRIVATE INTERFACE)

	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_args})
endfunction()

function(add_bve_library _target)
	set(_args ${ARGN})

	add_library(${_target} ${_args})
	
	target_add_sanitizers(${_target})
	target_add_warnings(${_target})
	target_bve_cotire(${_target})
	target_bve_coverage(${_target})
	target_format(${_target})

	list(REMOVE_ITEM _args STATIC SHARED OBJECT WIN32 PUBLIC PRIVATE INTERFACE)

	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_args})
endfunction()
