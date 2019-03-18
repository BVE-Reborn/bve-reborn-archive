include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NovaMods/nova-renderer
    REF 3b65755b8ef6f424b04ae71210f30c605e552ae9
    SHA512 f1908ab1e290c91a1d1556190c2e7d2c4425553f8e26680a6fe656739890a333d645a1ef638e9176ae3995ee87dc5ae0c205c340c63e8f5a63ef5cbca51d5d7a
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
