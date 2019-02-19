if (BVEREBORN_VTUNE)
	set(VTUNE_PATHS
		"C:/Program Files (x86)/IntelSWTools/VTune Amplifier"
		"C:/Program Files (x86)/IntelSWTools/VTune Amplifier 2019"
		"C:/Program Files (x86)/IntelSWTools/VTune Amplifier 2018"
		${BVEREBORN_VTUNE_ROOT}
	)

	find_path(VTune_INCLUDE_DIRS ittnotify.h 
		PATHS ${VTUNE_PATHS}
		PATH_SUFFIXES include
	)

	find_library(VTune_LIBRARIES libittnotify
		PATHS ${VTUNE_PATHS}
		PATH_SUFFIXES lib64
	)

	if(VTune_INCLUDE_DIRS AND VTune_LIBRARIES)
		add_library(vtune::vtune STATIC IMPORTED)
		set_target_properties(vtune::vtune PROPERTIES
			IMPORTED_LOCATION             ${VTune_LIBRARIES}
			INTERFACE_INCLUDE_DIRECTORIES ${VTune_INCLUDE_DIRS}
			INTERFACE_COMPILE_DEFINITIONS "BVE_VTUNE"
		)

		message(STATUS "Found VTune  ittnottify.h: ${VTune_INCLUDE_DIRS}/ittnottify.h")
		message(STATUS "Found VTune libittnottify: ${VTune_LIBRARIES}")
	endif()

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(VTune
		REQUIRED_VARS VTune_INCLUDE_DIRS VTune_LIBRARIES
	)
else()
	add_library(vtune::vtune INTERFACE IMPORTED)
endif()
