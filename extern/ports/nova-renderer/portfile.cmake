include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

if ((NOT (${TARGET_TRIPLET} MATCHES "x64")) OR (${TARGET_TRIPLET} MATCHES UWP))
	message(FATAL_ERROR "Foundational only supports x64 at this time.")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NovaMods/nova-renderer
    REF c6d27216798c1fd8180bf209c9a2a454edf8a967
    SHA512 6868532d0a32fc265bdcbee98d214e5e396f7924c809cd7363b0564242ccefef933e480c6187689d1d58af4a09ce2e4db123c2550d8f1c04056ac452b7cf09f6
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DNOVA_PACKAGE=True
)

vcpkg_install_cmake()
vcpkg_fixup_cmake_targets()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/share)

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/nova-renderer RENAME copyright)

vcpkg_copy_pdbs()
