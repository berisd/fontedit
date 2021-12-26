configure_file(fontedit_config.h.in fontedit_config.h)

add_library(video render.c render.h video.c video.h)

add_library(font font.c font.h util.c util.h)

add_library(widget widget/widget.c widget/widget.h widget/label.c widget/label.c widget/menu_item.c widget/menu_item.h
        widget/menu.c widget/menu.h widget/menu_bar.c widget/menu_bar.h widget/char_edit.c widget/char_edit.h
        widget/char_table.c widget/char_table.h widget/messagebox.c widget/messagebox.h widget/inputbox.c
        widget/inputbox.h widget/window.c widget/window.h theme.c theme.h color.h)

add_executable(fontedit fontedit.c gui.c gui.h)

target_link_libraries(video SDL2)
target_link_libraries(widget video font)
target_link_libraries(fontedit SDL2main SDL2 video widget font)

#target_include_directories(fontedit PRIVATE ${sdl2_SOURCE_DIRS}/include "${PROJECT_BINARY_DIR}/src" "${PROJECT_BINARY_DIR}/src/widget")

IF (WIN32)
    message(STATUS "Detected Win32!")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(MY_SDL2_LIB_NAME "SDL2d.dll")
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(MY_SDL2_LIB_NAME "SDL2.dll")
    ENDIF()
elseif(UNIX)
    message(STATUS "Detected Unix!")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(MY_SDL2_LIB_NAME "libSDL2-2.0d.so.0")
    ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(MY_SDL2_LIB_NAME "libSDL2-2.0.so.0")
    ENDIF()
ENDIF()

if (MY_SDL2_LIB_NAME)
    message("copy from ${CMAKE_CURRENT_BINARY_DIR}/../_deps/sdl2-build/${MY_SDL2_LIB_NAME} to ${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_command(TARGET fontedit POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_BINARY_DIR}/../_deps/sdl2-build/${MY_SDL2_LIB_NAME}"
            ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()