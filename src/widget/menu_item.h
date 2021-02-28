//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_ITEM_H
#define BRS_GUI_MENU_ITEM_H

#include "widget.h"

typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

struct _BRS_GUI_MenuItem {
    BRS_GUI_Widget widget;
    char *label;
    bool highlighted;
};

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

void BRS_GUI_MenuItem_ctor(BRS_GUI_MenuItem *menuItem, BRS_GUI_Widget_Properties *widgetProps, const char *label);

void BRS_GUI_MenuItem_dtor(BRS_GUI_MenuItem *menuItem);

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_GUI_Widget_Properties *widgetProps, const char *label);

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem);

#endif //BRS_GUI_MENU_ITEM_H
