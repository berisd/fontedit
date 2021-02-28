//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_BAR_H
#define BRS_GUI_MENU_BAR_H

#include "widget.h"

typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;

struct _BRS_GUI_MenuBar {
    BRS_GUI_Widget widget;
};

void BRS_GUI_MenuBar_ctor(BRS_GUI_MenuBar *menuBar, BRS_GUI_Widget_Properties *widgetProps);

void BRS_GUI_MenuBar_dtor(BRS_GUI_MenuBar *menuBar);

BRS_GUI_MenuBar *BRS_GUI_MenuBar_create(BRS_GUI_Widget_Properties *widgetProps);

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar);

#endif //BRS_GUI_MENU_BAR_H
