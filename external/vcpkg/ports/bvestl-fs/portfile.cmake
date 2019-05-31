include(vcpkg_common_functions)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO BVE-Reborn/bvestl-fs
    REF db83a2dfa98176dc5674fb0ebe728c22b85c7a03
    SHA512 2ae3f4d5257a865954ff927006e06ab063ada3a0cdc04ef757986a548beae6210adb2f388a7c61b9b9aef17ae92317d3a5be5a62ec66c4d39bedfc7a860ea9f3
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS -DBVESTL_FS_USER=On
)

vcpkg_install_cmake()

vcpkg_fixup_cmake_targets()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/bvestl-fs RENAME copyright)
