#!/bin/bash

if [[ -z "${VCPKG_ROOT}" ]]; then
	echo "ENV var VCPKG_ROOT not found. Please set it."
	exit 1
fi

echo "Installing nova-renderer port."
cp -r extern/ports/nova-renderer ${VCPKG_ROOT}/ports/

vcpkg install --recurse\
	abseil\
	concurrentqueue\
	cppfs\
	doctest\
	fmt\
	glm\
	mapbox-variant\
	ms-gsl\
	rapidxml-ns\
	nova-renderer
