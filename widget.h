//
// Created by bernd on 02.01.21.
//

#ifndef BRS_WIDGET_H
#define BRS_WIDGET_H

#include "video.h"
#include "list.h"

struct _BRS_GUI_MenuItem {
    char *label;
};
typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

struct _BRS_GUI_Menu {
    char *label;
    BRS_GUI_MenuItemList *itemList;
};
typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

BRS_LIST_DECL(BRS_GUI_MenuList, BRS_GUI_Menu)

struct _BRS_GUI_MenuBar {
    BRS_Point *position;
    BRS_Dimension *dimension;
    const BRS_Color *color;
    BRS_GUI_MenuList *menuList;
};
typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;

BRS_GUI_MenuBar *BRS_GUI_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color);

BRS_GUI_Menu *BRS_GUI_renderMenuBar(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar, BRS_Font *font);

BRS_GUI_Menu *BRS_GUI_createMenu();

BRS_GUI_MenuItem *BRS_GUI_createMenuItem();

#endif //BRS_WIDGET_H
