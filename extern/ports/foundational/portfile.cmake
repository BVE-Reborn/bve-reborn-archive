include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

if ((NOT (${TARGET_TRIPLET} MATCHES "x64")) OR (${TARGET_TRIPLET} MATCHES UWP))
	message(FATAL_ERROR "Foundational only supports x64 at this time.")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO NovaMods/foundational
    REF 0056f5921b761b6533628c4ca4d33154273e88db
    SHA512 658540067e124c1a2fd153a11114463ba7eb798015a72e2ec6ec7d30398c8033609768057122c879ddc4818e9fe6ae7dcefccba1d83c4c475eb15393d8f24ccd
    HEAD_REF master
)

vcpkg_download_distfile(
    CMAKE_HELPERS_PATH
    URLS https://github.com/NovaMods/cmake-helpers/archive/251d3e3917b510113b7466a3cbb84d1c793466c3.zip
    FILENAME NovaMods-cmake-helpers-251d3e3917b510113b7466a3cbb84d1c793466c3.zip
    SHA512 bd52eb1ef9b7ae7fe82dae34c508081b2b7cc6f71916c1c440bf05be4bbb4acd3b3e3cbb9be68b85a015afa49e311548f160df17acd86b246b67b5577a41d5e7
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
