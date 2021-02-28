//
// Created by bernd on 23.01.21.
//

#include "window.h"

void BRS_GUI_Window_ctor(BRS_GUI_Window *window, BRS_GUI_Widget_Properties *widgetProps) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) window, widgetProps);
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_Window_destroy;
}

void BRS_GUI_Window_dtor(BRS_GUI_Window *window) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) window);
}

BRS_GUI_Window *BRS_GUI_Window_create(BRS_GUI_Widget_Properties *widgetProps) {
    BRS_GUI_Window *window = malloc(sizeof(BRS_GUI_Window));
    BRS_GUI_Window_ctor(window, widgetProps);
    return window;
}

void BRS_GUI_Window_destroy(BRS_GUI_Window *window) {
    BRS_GUI_Window_dtor(window);
    free(window);
}