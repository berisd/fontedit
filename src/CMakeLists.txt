configure_file(fontedit_config.h.in fontedit_config.h)

add_library(video render.c render.h video.c video.h)

add_library(font font.c font.h util.c util.h)

add_library(widget widget/widget.c widget/widget.h widget/label.c widget/label.c widget/menu_item.c widget/menu_item.h
        widget/menu.c widget/menu.h widget/menu_bar.c widget/menu_bar.h widget/char_edit.c widget/char_edit.h
        widget/char_table.c widget/char_table.h widget/messagebox.c widget/messagebox.h widget/inputbox.c
        widget/inputbox.h widget/window.c widget/window.h theme.c theme.h color.h)

add_executable(fontedit fontedit.c gui.c gui.h)

target_link_libraries(video ${SDL2_LIBRARIES})
target_link_libraries(widget video font)
target_link_libraries(fontedit video widget font)

target_include_directories(fontedit PUBLIC "${PROJECT_BINARY_DIR}")