//
// Created by bernd on 05.01.21.
//

#include "menu_bar.h"

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme) {
    BRS_GUI_MenuBar *menubar = malloc(sizeof(BRS_GUI_MenuBar));
    menubar->theme = (BRS_GUI_Theme *)theme;
    menubar->size = BRS_copySize(size);
    menubar->menuList = BRS_GUI_MenuList_create();
    menubar->position = BRS_copyPoint(position);
    return menubar;
}

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_MenuListEntry *entry = menuBar->menuList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Menu *menu = entry->value;
        BRS_GUI_Menu_destroy(menu);
        entry = entry->next;
    }
    free(menuBar->size);
    free(menuBar->position);
    free(menuBar);
}

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar) {
    BRS_setColor(context, menubar->theme->menuBarColor);
    BRS_Rect r = {.x = menubar->position->x, .y = menubar->position->y, .width=menubar->size->width, .height=menubar->size->height};
    BRS_drawlFillRect(context, &r);

    BRS_GUI_MenuListEntry *entry = menubar->menuList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Menu *menu = entry->value;
        BRS_GUI_Menu_render(context, menu);
        entry = entry->next;
    }
}

static void processMouseButtonDown(BRS_GUI_MenuBar *menuBar, SDL_MouseButtonEvent *button) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        BRS_Rect menuBarRect = {.x = menuBar->position->x, .y = menuBar->position->y, .width = menuBar->size->width, .height = menuBar->size->height};

        if (BRS_PointInRect(&mousePoint, &menuBarRect)) {
            menuBar->clickHandler(menuBar);
        }
    }
}

bool BRS_GUI_MenuBar_processEvent(BRS_GUI_MenuBar *menuBar, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            processMouseButtonDown(menuBar, &event->button);
            break;
    }

    BRS_GUI_MenuListEntry *menuEntry = menuBar->menuList->firstEntry;
    while (menuEntry != NULL) {
        BRS_GUI_Menu *menu = menuEntry->value;
        BRS_GUI_Menu_processEvent(menu, event);
        menuEntry = menuEntry->next;
    }
    return false;
}

BRS_Point *BRS_GUI_MenuBar_getPosition(BRS_GUI_MenuBar *menuBar) {
    return menuBar->position;
}

int32_t BRS_GUI_MenuBar_getMenuIndex(BRS_GUI_MenuBar *menuBar, BRS_GUI_Menu *menu) {
    int32_t index = -1;
    if (menuBar->menuList != NULL) {
        BRS_GUI_MenuListEntry *menuEntry = menuBar->menuList->firstEntry;
        while (menuEntry != NULL) {
            index++;
            if (menuEntry->value == menu) {
                break;
            }
            menuEntry = menuEntry->next;
        }
    }
    return index;
}
