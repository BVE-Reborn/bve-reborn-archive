function(target_add_warnings target)
	if (OS_WINDOWS)
		target_compile_options_if_supported(${target} PRIVATE /W4)
		target_compile_options_if_supported(${target} PRIVATE 
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
		target_compile_options_if_supported(${target} PRIVATE /experimental:external /external:anglebrackets /external:W3)
		target_compile_options_if_supported(${target} PRIVATE /WX)
		target_compile_options_if_supported(${target} PRIVATE /permissive-)
	else()
		target_compile_options_if_supported(${target} PRIVATE 
			-fdiagnostics-color=always
			-Wall
			-Wcast-align
			-Wcast-qual
			-Wconversion
			-Wctor-dtor-privacy
			-Wdisabled-optimization
			-Wdouble-promotion
			-Wduplicated-branches
			-Wduplicated-cond
			-Wextra
			-Wformat=2
			-Winit-self
			-Wlogical-op
			-Wno-gnu-zero-variadic-macro-arguments
			-Wmissing-include-dirs
			-Wno-sign-conversion
			-Wnoexcept
			-Wnull-dereference
			-Wold-style-cast
			-Woverloaded-virtual
			-Wpedantic
			-Wrestrict
			-Wshadow
			-Wstrict-aliasing=1
			-Wstrict-null-sentinel
			-Wstrict-overflow=2
			-Wswitch-default
			-Wundef
			-Wno-unknown-pragmas
			-Wuseless-cast
		) 
	endif()
endfunction()
