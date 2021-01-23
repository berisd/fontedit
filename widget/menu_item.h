//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_ITEM_H
#define BRS_GUI_MENU_ITEM_H

#include "../list.h"
#include "../render.h"
#include "../theme.h"

// External
typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;
// External End

typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

typedef void (*BRS_GUI_MenuItem_ClickHandler)(BRS_GUI_MenuItem *);

struct _BRS_GUI_MenuItem {
    BRS_GUI_Menu *menu;
    BRS_Size *size;
    const char *label;
    BRS_GUI_Theme *theme;
    BRS_GUI_MenuItem_ClickHandler clickHandler;
    bool highlighted;
};

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

// External
extern void BRS_GUI_Menu_calcPosition(BRS_GUI_Menu *menu, BRS_Point *menuPosition);

extern BRS_Size *BRS_GUI_Menu_getSize(BRS_GUI_Menu *menu);

extern int32_t BRS_GUI_Menu_getMenuItemIndex(BRS_GUI_Menu *menu, BRS_GUI_MenuItem *menuItem);
// External End

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_Size *size, BRS_GUI_Menu *menu, const char *label, const BRS_GUI_Theme *theme);

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem);

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem);

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler);

void BRS_GUI_MenuItem_processEvent(BRS_GUI_MenuItem *menuItem, SDL_Event *event);

#endif //BRS_GUI_MENU_ITEM_H
