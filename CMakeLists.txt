cmake_minimum_required(VERSION 3.16)
project(fontedit C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(fontedit fontedit.c video.c font.c util.c widget.c)

target_link_libraries(fontedit ${SDL2_LIBRARIES})