/**
 * Window displayed on GUI
 */

#ifndef FONTEDIT_WINDOW_H
#define FONTEDIT_WINDOW_H

#include <stdlib.h>
#include "widget.h"

typedef struct _BRS_GUI_Window {
    BRS_GUI_Widget widget;
} BRS_GUI_Window;

void BRS_GUI_Window_ctor(BRS_GUI_Window *window, BRS_GUI_Widget_Properties *widgetProps);

void BRS_GUI_Window_dtor(BRS_GUI_Window *window);

BRS_GUI_Window *
BRS_GUI_Window_create(BRS_GUI_Widget_Properties *properties);

void BRS_GUI_Window_destroy(BRS_GUI_Window *window);

#endif //FONTEDIT_WINDOW_H
