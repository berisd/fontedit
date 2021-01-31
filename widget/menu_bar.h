//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_BAR_H
#define BRS_GUI_MENU_BAR_H

#include "menu.h"
#include "widget.h"

typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;

typedef void (*BRS_GUI_MenuBar_ClickHandler)(BRS_GUI_MenuBar *);

struct _BRS_GUI_MenuBar {
    BRS_GUI_Widget *widget;
    BRS_GUI_MenuList *menuList;
    BRS_GUI_MenuBar_ClickHandler clickHandler;
};

BRS_GUI_MenuBar *BRS_GUI_MenuBar_create();

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar);

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

bool BRS_GUI_MenuBar_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

BRS_GUI_MenuBar *BRS_GUI_MenuBar_getFromWidget(BRS_GUI_Widget *widget);

#endif //BRS_GUI_MENU_BAR_H
