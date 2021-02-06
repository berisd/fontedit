//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_H
#define BRS_GUI_MENU_H

#include <stdbool.h>
#include "../video.h"
#include "../theme.h"
#include "widget.h"
#include "menu_item.h"

typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

struct _BRS_GUI_Menu {
    const char *label;
    BRS_GUI_MenuItemList *itemList;
};

BRS_LIST_DECL(BRS_GUI_MenuList, BRS_GUI_Menu)

const char *BRS_GUI_MENU_ACTION_HIDE;

BRS_GUI_Menu *
BRS_GUI_Menu_create(const char *label);

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu);

void BRS_GUI_Menu_calculate(BRS_GUI_Widget *menuWidget, BRS_GUI_Widget *parentWidget,
                            int32_t widgetIndex);

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Widget *menuWidget);

bool BRS_GUI_Menu_processEvent(BRS_GUI_Widget *menuWidget, SDL_Event *event);

void BRS_GUI_Menu_hide(BRS_GUI_Widget *menuWidget);

#endif //BRS_GUI_MENU_H
