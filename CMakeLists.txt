cmake_minimum_required(VERSION 3.16)
project(fontedit LANGUAGES C VERSION 0.1)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

configure_file(fontedit_config.h.in fontedit_config.h)

add_executable(fontedit fontedit.c
        video.c video.h font.c font.h util.c util.h gui.c gui.h
        widget/widget.c widget/widget.h widget/label.c widget/label.c widget/menu_item.c widget/menu_item.h
        widget/menu.c widget/menu.h widget/menu_bar.c widget/menu_bar.h widget/char_edit.c  widget/char_edit.h
        widget/char_table.c widget/char_table.h theme.c theme.h widget/messagebox.c widget/messagebox.h
        widget/inputbox.c widget/inputbox.h widget/window.c widget/window.h render.c render.h color.h)

target_link_libraries(fontedit ${SDL2_LIBRARIES})

target_include_directories(fontedit PUBLIC "${PROJECT_BINARY_DIR}")