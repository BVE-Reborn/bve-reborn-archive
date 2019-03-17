function(target_add_warnings target)
	if (OS_WINDOWS)
		target_compile_options_if_supported(${target} PUBLIC /W4)
		target_compile_options_if_supported(${target} PUBLIC 
			/w44061
			/w44062
			/w44263
			/w44264
			/w44266
			/w44296
			/w44545
			/w44546
			/w44547
			/w44548
			/w44549
			/w44574
			/w44668
			/wd4251
		)
		target_compile_options_if_supported(${target} PUBLIC /experimental:external /external:anglebrackets /external:W3)
		target_compile_options_if_supported(${target} PUBLIC /WX)
		target_compile_options_if_supported(${target} PUBLIC /permissive-)
	else()

	endif()
endfunction()
