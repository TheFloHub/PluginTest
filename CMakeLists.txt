cmake_minimum_required(VERSION 3.10)
project(PluginTest)
add_subdirectory(src) 

# build command:
# cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_TOOLCHAIN_FILE="D:\Repositories\vcpkg\scripts\buildsystems\vcpkg.cmake" ..