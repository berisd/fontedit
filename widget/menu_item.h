//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_ITEM_H
#define BRS_GUI_MENU_ITEM_H

#include "../list.h"
#include "../render.h"
#include "../theme.h"

typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

typedef void (*BRS_GUI_MenuItem_ClickHandler)(BRS_GUI_MenuItem *);

struct _BRS_GUI_MenuItem {
    BRS_Point *position;
    BRS_Size *size;
    const char *label;
    BRS_GUI_MenuItem_ClickHandler clickHandler;
    bool highlighted;
};

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_Size *size, const char *label);

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem);

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler);

#endif //BRS_GUI_MENU_ITEM_H
