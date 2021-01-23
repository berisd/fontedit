cmake_minimum_required(VERSION 3.16)
project(fontedit C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(fontedit fontedit.c video.c font.c util.c gui.c widget/widget.c widget/label.c widget/menu_item.c widget/menu.c widget/menu_bar.c widget/char_edit.c widget/char_table.c theme.c widget/messagebox.c widget/inputbox.c widget/window.c render.c render.h color.h)

target_link_libraries(fontedit ${SDL2_LIBRARIES})