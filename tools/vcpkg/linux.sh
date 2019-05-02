#!/bin/bash

if [[ -z "${VCPKG_ROOT}" ]]; then
	echo "ENV var VCPKG_ROOT not found. Please set it."
	exit 1
fi

cp -r extern/ports/foundational ${VCPKG_ROOT}/ports/
cp -r extern/ports/nova-renderer ${VCPKG_ROOT}/ports/

vcpkg update
vcpkg upgrade --no-dry-run
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