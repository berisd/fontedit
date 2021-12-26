cmake_minimum_required(VERSION 3.16)
project(fontedit LANGUAGES C VERSION 0.1)

set(CMAKE_C_STANDARD 11)

if (UNIX)
    set(CMAKE_BUILD_RPATH  $ORIGIN)
endif()

include(cmake/CPM.cmake)
CPMAddPackage(
        NAME SDL2
        VERSION 2.0.16
        URL https://www.libsdl.org/release/SDL2-2.0.16.zip
)
CPMAddPackage(
        NAME CMockA
        VERSION 1.1.5
        URL https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz
)

find_package(SDL2 REQUIRED)
find_package(CMockA)

include(CTest)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
