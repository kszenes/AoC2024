set(CMAKE_TOOLCHAIN_FILE $ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake)
cmake_minimum_required(VERSION 3.21)

project(fmt_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(main main.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt-header-only)
