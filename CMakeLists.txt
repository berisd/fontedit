cmake_minimum_required(VERSION 3.16)
project(fontedit LANGUAGES C VERSION 0.1)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(CMockA)

include(CTest)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
