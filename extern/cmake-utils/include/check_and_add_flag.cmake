include(CMakeParseArguments)
include(CheckCXXCompilerFlag)

# Function to add flag if compiler supports it
function(check_and_add_flag)
	cmake_parse_arguments(CAAF "" "FLAG" "" ${ARGN})
	# Sanitize flag to become cmake variable name
	string (REPLACE "-" "_" NAME ${CAAF_FLAG})
	string (SUBSTRING ${NAME} 1 -1 NAME)
	string (FIND ${NAME} "=" EQUALS)
	string (SUBSTRING ${NAME} 0 ${EQUALS} NAME)
	CHECK_CXX_COMPILER_FLAG(${CAAF_FLAG} ${NAME})
	if (${NAME}) 
		add_compile_options(${CAAF_FLAG})
	endif()
endfunction()