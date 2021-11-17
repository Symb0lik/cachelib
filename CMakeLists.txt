# CMakeList.txt : CMake project for cachelib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")
project ("cachelib")


# Add source to this project's executable.
add_subdirectory("cachetool")
add_subdirectory("lib")
add_subdirectory("test")

# TODO: Add tests and install targets if needed.
