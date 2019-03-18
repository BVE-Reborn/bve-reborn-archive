function(add_bve_executable _target)
	set(_args ${ARGN})

	if(ARGC GREATER_EQUAL 2)
		if(${ARGV1} MATCHES "STATIC|SHARED|OBJECT|WIN32|PUBLIC|PRIVATE|INTERFACE")		
			set(_type ${ARGV1})
		endif()
	endif()
	
	list(REMOVE_ITEM _args STATIC SHARED OBJECT WIN32 PUBLIC PRIVATE INTERFACE)

	add_executable(${_target} ${_type} ${_args})
	
	target_add_sanitizers(${_target})
	target_add_warnings(${_target})
	target_bve_coverage(${_target})
	target_format(${_target})

	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_args})
endfunction()

function(add_bve_library _target)
	set(_args ${ARGN})

	if(ARGC GREATER_EQUAL 2)
		if(${ARGV1} MATCHES "STATIC|SHARED|OBJECT|WIN32|PUBLIC|PRIVATE|INTERFACE")		
			set(_type ${ARGV1})
		endif()
	endif()
	
	list(REMOVE_ITEM _args STATIC SHARED OBJECT WIN32 PUBLIC PRIVATE INTERFACE)

	add_library(${_target} ${_type} ${_args})
	
	target_add_sanitizers(${_target})
	target_add_warnings(${_target})
	target_bve_coverage(${_target})
	target_format(${_target})

	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_args})
endfunction()

function(finish_bve_target _target)
	target_bve_cotire(${_target})
endfunction()
