//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_BAR_H
#define BRS_GUI_MENU_BAR_H

#include "menu.h"

typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;

typedef void (*BRS_GUI_MenuBar_ClickHandler)(BRS_GUI_MenuBar *);

struct _BRS_GUI_MenuBar {
    BRS_Font *font;
    BRS_Point *position;
    BRS_Dimension *dimension;
    const BRS_Color *color;
    BRS_GUI_MenuList *menuList;
    BRS_GUI_MenuBar_ClickHandler clickHandler;
};

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color, BRS_Font *font);

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar);

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar);

void BRS_GUI_MenuBar_processEvent(BRS_GUI_MenuBar *menuBar, SDL_Event *event);

BRS_Point *BRS_GUI_MenuBar_getPosition(BRS_GUI_MenuBar *menuBar);

/**
 * Get Index for menu in menubar
 * @param menuBar
 * @param menu
 * @return index starting at 0. -1 if menu not found.
 */
int32_t BRS_GUI_MenuBar_getMenuIndex(BRS_GUI_MenuBar *menuBar, BRS_GUI_Menu *menu);

#endif //BRS_GUI_MENU_BAR_H
