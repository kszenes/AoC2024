set(CMAKE_TOOLCHAIN_FILE $ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
cmake_minimum_required(VERSION 3.21)

project(day07 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt-header-only)
