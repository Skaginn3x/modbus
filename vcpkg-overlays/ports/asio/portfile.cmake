#header-only library

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO chriskohlhoff/asio
    REF 63a1fbff7c9b4d615fffd81d42cba82aff1319a3
    SHA512 98edbc561e5de4ff694ba3b7d5212e4d2dfb6748c0fdd79a429c4389d17319d701aee5d48344d08bbdbac8feb2200ca90451daf738525169dc71a9e9fb348494
    HEAD_REF master
)

# Always use "ASIO_STANDALONE" to avoid boost dependency
vcpkg_replace_string("${SOURCE_PATH}/asio/include/asio/detail/config.hpp" "defined(ASIO_STANDALONE)" "!defined(VCPKG_DISABLE_ASIO_STANDALONE)")

# CMake install
file(COPY "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt" DESTINATION "${SOURCE_PATH}")
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)
vcpkg_cmake_install()

vcpkg_cmake_config_fixup()
file(INSTALL
    "${CMAKE_CURRENT_LIST_DIR}/asio-config.cmake"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")

# Handle copyright
file(INSTALL "${SOURCE_PATH}/asio/LICENSE_1_0.txt" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
