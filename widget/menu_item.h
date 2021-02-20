//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_ITEM_H
#define BRS_GUI_MENU_ITEM_H

#include "../list.h"
#include "../render.h"
#include "../theme.h"
#include "widget.h"

typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

struct _BRS_GUI_MenuItem {
    const char *label;
    bool highlighted;
};

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(const char *label);

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem);

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

bool BRS_GUI_MenuItem_processEvent(BRS_GUI_Widget *menuItem, SDL_Event *event);

#endif //BRS_GUI_MENU_ITEM_H
