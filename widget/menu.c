//
// Created by bernd on 05.01.21.
//

#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

static uint16_t _countMenuItems(BRS_GUI_Menu *menu) {
    uint16_t count = 0;
    BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
    while (entry != NULL) {
        count++;
        entry = entry->next;
    }
    return count;
}

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Menu *menu) {
    BRS_Point position;
    BRS_GUI_Menu_calcPosition(menu, &position);
    BRS_setColor(context, menu->theme->menuBackColor);
    BRS_Rect menuRect = {.x = position.x, .y = position.y, .width = menu->dimension->width, .height = menu->dimension->height};
    BRS_drawlFillRect(context, &menuRect);

    BRS_drawString(context, menu->label, strlen(menu->label), menu->theme->font, &position,
                   menu->selected ? menu->theme->menuSelectedForeColor : menu->theme->menuForeColor);

    if (menu->selected) {
        BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
        while (entry != NULL) {
            BRS_GUI_MenuItem_render(context, entry->value);
            entry = entry->next;
        }
    }
}

BRS_GUI_Menu *
BRS_GUI_Menu_create(BRS_GUI_MenuBar *menuBar, BRS_Dimension *dimension, const char *label, const BRS_GUI_Theme *theme,
                    bool selected) {
    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->menuBar = menuBar;
    menu->dimension = BRS_copyDimension(dimension);
    menu->label = label;
    menu->theme = (BRS_GUI_Theme *)theme;
    menu->itemList = BRS_GUI_MenuItemList_create();
    menu->selected = selected;
    menu->itemList = BRS_GUI_MenuItemList_create();
    return menu;
}

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu) {
    BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_MenuItem *menuItem = entry->value;
        BRS_GUI_MenuItem_destroy(menuItem);
        entry = entry->next;
    }
    free(menu->dimension);
    free(menu);
}

void BRS_GUI_Menu_processEvent(BRS_GUI_Menu *menu, SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION && menu->selected) {
        BRS_Point position;
        BRS_GUI_Menu_calcPosition(menu, &position);

        BRS_Dimension *menuItemDimension = menu->itemList->firstEntry->value->dimension;
        int16_t menuItemsHeight = _countMenuItems(menu) * menuItemDimension->height;

        BRS_Rect menuRect = {.x = position.x, .y = position.y, .width = menu->dimension->width, .height =
        menu->dimension->height + menuItemsHeight};

        BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
        if (!BRS_PointInRect(&mousePoint, &menuRect)) {
            menu->selected = false;
        }
    }
    if (!menu->selected) {
        return;
    }

    BRS_GUI_MenuItemListEntry *menuItemEntry = menu->itemList->firstEntry;
    while (menuItemEntry != NULL) {
        BRS_GUI_MenuItem *menu = menuItemEntry->value;
        BRS_GUI_MenuItem_processEvent(menu, event);
        menuItemEntry = menuItemEntry->next;
    }
}

BRS_Dimension *BRS_GUI_Menu_getDimension(BRS_GUI_Menu *menu) {
    return menu->dimension;
}

void BRS_GUI_Menu_calcPosition(BRS_GUI_Menu *menu, BRS_Point *menuPosition) {
    BRS_Point *menuBarPos = BRS_GUI_MenuBar_getPosition(menu->menuBar);
    int32_t menuIndex = BRS_GUI_MenuBar_getMenuIndex(menu->menuBar, menu);
    menuPosition->x = menuBarPos->x + menu->dimension->width * menuIndex;
    menuPosition->y = menuBarPos->y;
}

int32_t BRS_GUI_Menu_getMenuItemIndex(BRS_GUI_Menu *menu, BRS_GUI_MenuItem *menuItem) {
    int32_t index = -1;
    if (menu->itemList != NULL) {
        BRS_GUI_MenuItemListEntry *menuItemEntry = menu->itemList->firstEntry;
        while (menuItemEntry != NULL) {
            index++;
            if (menuItemEntry->value == menuItem) {
                break;
            }
            menuItemEntry = menuItemEntry->next;
        }
    }
    return index;
}