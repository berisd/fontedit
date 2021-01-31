//
// Created by bernd on 05.01.21.
//

#include "menu_bar.h"

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create() {
    BRS_GUI_MenuBar *menubar = malloc(sizeof(BRS_GUI_MenuBar));
    menubar->menuList = BRS_GUI_MenuList_create();
    return menubar;
}

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_MenuListEntry *entry = menuBar->menuList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Menu *menu = entry->value;
        BRS_GUI_Menu_destroy(menu);
        entry = entry->next;
    }
    free(menuBar);
}

static uint16_t _countMenuItems(BRS_GUI_Menu *menu) {
    uint16_t count = 0;
    BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
    while (entry != NULL) {
        count++;
        entry = entry->next;
    }
    return count;
}

static void
BRS_GUI_Menu_calcPosition(BRS_GUI_Menu *menu, BRS_Point *menuPosition, int32_t menuIndex, BRS_Point *menuBarPos) {
    menuPosition->x = menuBarPos->x + menu->size->width * menuIndex;
    menuPosition->y = menuBarPos->y;
}

static void calculateMenuItemPosition(BRS_GUI_MenuItem *menuItem, BRS_Point *menuItemPosition, int32_t menuItemIndex,
                                      BRS_GUI_Menu *menu, int32_t menuIndex, BRS_Point *menuBarPos) {
    BRS_Point menuPosition;
    BRS_GUI_Menu_calcPosition(menu, &menuPosition, menuIndex, menuBarPos);

    menuItemPosition->x = menuPosition.x;
    menuItemPosition->y = menuPosition.y + menu->size->height + menuItem->size->height * menuItemIndex;
}


static void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem, int32_t menuItemIndex,
                                    BRS_GUI_Menu *menu, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    BRS_Point position;
    calculateMenuItemPosition(menuItem, &position, menuItemIndex, menu, menuIndex, menuBarWidget->position);

    BRS_setColor(context, menuBarWidget->theme->menuItemBackColor);
    BRS_Rect menuItemRect = {.x = position.x, .y = position.y, .width=menuItem->size->width, .height=menuItem->size->height};
    BRS_drawlFillRect(context, &menuItemRect);

    BRS_setColor(context, menuItem->highlighted ? menuBarWidget->theme->menuItemHighlightedColor
                                                : menuBarWidget->theme->menuItemForeColor);
    BRS_drawString(context, menuItem->label, strlen(menuItem->label), menuBarWidget->theme->font, &position);
}

static void
BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Menu *menu, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    BRS_Point position;
    BRS_GUI_Menu_calcPosition(menu, &position, menuIndex, menuBarWidget->position);
    BRS_setColor(context, menuBarWidget->theme->menuBackColor);
    BRS_Rect menuRect = {.x = position.x, .y = position.y, .width = menu->size->width, .height = menu->size->height};
    BRS_drawlFillRect(context, &menuRect);

    BRS_setColor(context,
                 menu->selected ? menuBarWidget->theme->menuSelectedForeColor : menuBarWidget->theme->menuForeColor);
    BRS_drawString(context, menu->label, strlen(menu->label), menuBarWidget->theme->font, &position);

    if (menu->selected) {
        int32_t menuItemIndex = 0;
        BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
        while (entry != NULL) {
            BRS_GUI_MenuItem_render(context, entry->value, menuItemIndex, menu, menuIndex, menuBarWidget);
            entry = entry->next;
            menuItemIndex++;
        }
    }
}

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_MenuBar *menuBar = widget->object;
    BRS_setColor(context, widget->theme->menuBarColor);
    BRS_Rect r = {.x = widget->position->x, .y = widget->position->y, .width=widget->size->width, .height=widget->size->height};
    BRS_drawlFillRect(context, &r);

    BRS_GUI_MenuListEntry *entry = menuBar->menuList->firstEntry;
    int32_t menuIndex = 0;
    while (entry != NULL) {
        BRS_GUI_Menu *menu = entry->value;
        BRS_GUI_Menu_render(context, menu, menuIndex, widget);
        entry = entry->next;
        menuIndex++;
    }
}

static void processMouseButtonDown(BRS_GUI_Widget *widget, SDL_MouseButtonEvent *button) {
    BRS_GUI_MenuBar *menuBar = widget->object;
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        BRS_Rect menuBarRect = {.x = widget->position->x, .y = widget->position->y, .width = widget->size->width, .height = widget->size->height};

        if (BRS_PointInRect(&mousePoint, &menuBarRect)) {
            menuBar->clickHandler(menuBar);
        }
    }
}

static void
processMenuItemMouseButtonDown(BRS_GUI_MenuItem *menuItem, SDL_MouseButtonEvent *button, int32_t menuItemIndex,
                               BRS_GUI_Menu *menu, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Point menuPosition;
        BRS_GUI_Menu_calcPosition(menu, &menuPosition, menuIndex, menuBarWidget->position);
        BRS_Point menuItemPosition;
        calculateMenuItemPosition(menuItem, &menuItemPosition, menuItemIndex, menu, menuIndex, menuBarWidget->position);

        BRS_Rect menuItemRect = {.x = menuItemPosition.x, .y = menuItemPosition.y, .width = menuItem->size->width,
                menuItem->size->height};
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        bool clickedInMenuItem = BRS_PointInRect(&mousePoint, &menuItemRect);

        if (clickedInMenuItem && menuItem->clickHandler) {
            menuItem->clickHandler(menuItem);
        }
    }
}

static void processMenuItemMouseMove(BRS_GUI_MenuItem *menuItem, SDL_MouseMotionEvent *motion, int32_t menuItemIndex,
                                     BRS_GUI_Menu *menu, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    BRS_Point mousePoint = {.x = motion->x, .y = motion->y};
    BRS_Point position;
    calculateMenuItemPosition(menuItem, &position, menuItemIndex, menu, menuIndex, menuBarWidget->position);
    BRS_Rect widgetRect = {.x = position.x, .y = position.y, .width = menuItem->size->width, .height = menuItem->size->height};
    menuItem->highlighted = BRS_PointInRect(&mousePoint, &widgetRect);
}

static void
BRS_GUI_MenuItem_processEvent(BRS_GUI_MenuItem *menuItem, SDL_Event *event, int32_t menuItemIndex, BRS_GUI_Menu *menu,
                              int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            processMenuItemMouseMove(menuItem, &event->motion, menuItemIndex, menu, menuIndex, menuBarWidget);
            break;
        case SDL_MOUSEBUTTONUP:
            processMenuItemMouseButtonDown(menuItem, &event->button, menuItemIndex, menu, menuIndex, menuBarWidget);
            break;
    }
}

static void
BRS_GUI_Menu_processEvent(BRS_GUI_Menu *menu, SDL_Event *event, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    if (event->type == SDL_MOUSEMOTION && menu->selected) {
        BRS_Point position;
        BRS_GUI_Menu_calcPosition(menu, &position, menuIndex, menuBarWidget->position);

        BRS_Size *menuItemSize = menu->itemList->firstEntry->value->size;
        int16_t menuItemsHeight = _countMenuItems(menu) * menuItemSize->height;

        BRS_Rect menuRect = {.x = position.x, .y = position.y, .width = menu->size->width, .height =
        menu->size->height + menuItemsHeight};

        BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
        if (!BRS_PointInRect(&mousePoint, &menuRect)) {
            menu->selected = false;
        }
    }
    if (!menu->selected) {
        return;
    }

    BRS_GUI_MenuItemListEntry *menuItemEntry = menu->itemList->firstEntry;
    int32_t menuItemIndex = 0;
    while (menuItemEntry != NULL) {
        BRS_GUI_MenuItem *menuItem = menuItemEntry->value;
        BRS_GUI_MenuItem_processEvent(menuItem, event, menuItemIndex, menu, menuIndex, menuBarWidget);
        menuItemEntry = menuItemEntry->next;
        menuItemIndex++;
    }
}

bool BRS_GUI_MenuBar_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    BRS_GUI_MenuBar *menuBar = widget->object;
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            processMouseButtonDown(widget, &event->button);
            break;
    }

    BRS_GUI_MenuListEntry *menuEntry = menuBar->menuList->firstEntry;
    int32_t menuIndex = 0;
    while (menuEntry != NULL) {
        BRS_GUI_Menu *menu = menuEntry->value;
        BRS_GUI_Menu_processEvent(menu, event, menuIndex, widget);
        menuEntry = menuEntry->next;
        menuIndex++;
    }
    return false;
}

BRS_GUI_MenuBar *BRS_GUI_MenuBar_getFromWidget(BRS_GUI_Widget *widget) {
    return widget->object;
}
