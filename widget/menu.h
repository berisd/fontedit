//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_H
#define BRS_GUI_MENU_H

#include <stdbool.h>
#include "../video.h"
#include "menu_item.h"

// External
typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;

typedef void (*BRS_GUI_MenuBar_ClickHandler)(BRS_GUI_MenuBar *);
// External End

typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

struct _BRS_GUI_Menu {
    BRS_GUI_MenuBar *menuBar;
    BRS_Dimension *dimension;
    const char *label;
    const BRS_Color *foreColor;
    const BRS_Color *backColor;
    const BRS_Color *selectedForeColor;
    BRS_Font *font;
    bool selected;
    BRS_GUI_MenuItemList *itemList;
};

BRS_LIST_DECL(BRS_GUI_MenuList, BRS_GUI_Menu)

// External
BRS_Point *BRS_GUI_MenuBar_getPosition(BRS_GUI_MenuBar *menuBar);

BRS_Dimension *BRS_GUI_MenuBar_getDimension(BRS_GUI_MenuBar *menuBar);

BRS_GUI_MenuBar_ClickHandler BRS_GUI_MenuBar_getClickHandler(BRS_GUI_MenuBar *menuBar);
int32_t BRS_GUI_MenuBar_getMenuIndex(BRS_GUI_MenuBar *menuBar, BRS_GUI_Menu *menu);
// External End

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Menu *menu, BRS_Point position);

BRS_GUI_Menu *
BRS_GUI_Menu_create(BRS_GUI_MenuBar *menuBar, BRS_Dimension *dimension, const char *label, const BRS_Color *foreColor,
                    const BRS_Color *backColor, const BRS_Color *selectedForeColor, BRS_Font *font, bool selected);

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu);

void BRS_GUI_Menu_processEvent(BRS_GUI_Menu *menu, SDL_Event *event);

BRS_Dimension *BRS_GUI_Menu_getDimension(BRS_GUI_Menu *menu);

BRS_Point *BRS_GUI_Menu_calcPosition(BRS_GUI_Menu *menu);

/**
 * Get Index for menuitem in menu
 * @param menu
 * @param menuItem
 * @return index starting at 0. -1 if menuitem not found.
 */
int32_t BRS_GUI_Menu_getMenuItemIndex(BRS_GUI_Menu *menu, BRS_GUI_MenuItem *menuItem);

#endif //BRS_GUI_MENU_H
