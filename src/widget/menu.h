//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_H
#define BRS_GUI_MENU_H

#include <stdbool.h>
#include "widget.h"

typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

struct _BRS_GUI_Menu {
    BRS_GUI_Widget widget;
    char *label;
};

const char *BRS_GUI_MENU_ACTION_HIDE;

BRS_GUI_Menu *BRS_GUI_Menu_create(BRS_GUI_Widget_Properties *widgetProps, const char *label);

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu);

void BRS_GUI_Menu_ctor(BRS_GUI_Menu *menu, BRS_GUI_Widget_Properties *widgetProps, const char *label);

void BRS_GUI_Menu_dtor(BRS_GUI_Menu *menu);

void BRS_GUI_Menu_hide(BRS_GUI_Menu *menu);

#endif //BRS_GUI_MENU_H
