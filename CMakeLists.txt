cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(voxelius LANGUAGES C CXX VERSION 0.0.1.2453)

set(CPACK_PACKAGE_NAME "voxelius")
set(CPACK_PACKAGE_VENDOR "untolabs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Block game on steroids")
set(CPACK_PACKAGE_CONTACT "https://github.com/untolabs/voxelius/issues")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})

set(CPACK_PACKAGE_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")

# This enables solution directories if the feature is applicable
# to the target build environment. This allows built-in pseudo-targets
# like ALL_BUILD and ZERO_CHECK to be moved out of sight into a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BUILD_CLIENT ON CACHE BOOL "Build Voxelius client executable")
set(BUILD_SERVER ON CACHE BOOL "Build Voxelius server executable")

set(ENABLE_EXPERIMENTS ON CACHE BOOL "Enable basic experimental features")

# Ensure we're statically linking to all the dependencies that we
# pull and build by ourselves; I cannot guarantee that the packaging
# scripts for non-Windows systems will simply work out of the box
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# Ensure we're also statically linking with MSVC runtime libraries; this way
# the game's binaries can be distributed without needing VC++ distributables
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Output game binaries into a single directory for ease of access
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# Require ISO C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Require ISO C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(assets)

add_subdirectory(external)

add_subdirectory(source/cmake)
add_subdirectory(source/common)
add_subdirectory(source/mathlib)

add_subdirectory(source/game/client)
add_subdirectory(source/game/server)
add_subdirectory(source/game/shared)

install(FILES "${CMAKE_CURRENT_LIST_DIR}/.itch.toml" DESTINATION ".")
install(FILES "${CMAKE_CURRENT_LIST_DIR}/LICENSE.txt" DESTINATION ".")
install(FILES "${CMAKE_CURRENT_LIST_DIR}/LICENSE_assets.txt" DESTINATION ".")

include(CPack)
