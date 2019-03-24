include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

if ((NOT (${TARGET_TRIPLET} MATCHES "x64")) OR (${TARGET_TRIPLET} MATCHES UWP))
	message(FATAL_ERROR "Foundational only supports x64 at this time.")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NovaMods/foundational
    REF d4aa07979ad94dc22f9258babc09829abeb3b330
    SHA512 0e0c3983c8264a38f6c405204886154bd8acf76330b334e29c9f7ae4bbe173f87dc6629f27cb38a9b788cc0234c6e91c0b54af6e4e12f60ca29b7a844838308d
    HEAD_REF master
)

vcpkg_download_distfile(
    CMAKE_HELPERS_PATH
    URLS https://github.com/NovaMods/cmake-helpers/archive/1cfce8b1bc40dfbbc6935f92ed39a873b507ffe4.zip
    FILENAME NovaMods-cmake-helpers-1cfce8b1bc40dfbbc6935f92ed39a873b507ffe4.zip
    SHA512 20732a1087f0a288db45d64794ad564dab61f63c91dbdf6cad41b4acaa1515ac1a44cd1a6df1c79d590a84017c19d7914c35aed648f6a2c509aab1b2a312fda2
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH CMAKE_HELPERS_SOURCE_PATH
    ARCHIVE "${CMAKE_HELPERS_PATH}"
    WORKING_DIRECTORY "${SOURCE_PATH}/external/cmake-helpers"
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DNOVA_PACKAGE=True
        "-DCMAKE_MODULE_PATH=${CMAKE_HELPERS_SOURCE_PATH}"
)

vcpkg_install_cmake()
vcpkg_fixup_cmake_targets()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/share)

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/foundational RENAME copyright)

vcpkg_copy_pdbs()
