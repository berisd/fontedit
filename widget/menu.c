//
// Created by bernd on 05.01.21.
//

#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Menu *menu) {
    BRS_Point *position = BRS_GUI_Menu_calcPosition(menu);
    BRS_setColor(context, menu->backColor);
    SDL_Rect r = {.x = position->x, .y = position->y, .w=menu->dimension->width, .h=menu->dimension->height};
    SDL_RenderFillRect(context->renderer, &r);

    position->x++;
    position->y++;
    BRS_drawString(context, menu->label, menu->font, position,
                   menu->selected ? menu->selectedForeColor : menu->foreColor);

    if (menu->selected) {
        BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
        while (entry != NULL) {
            BRS_GUI_MenuItem_render(context, entry->value);
            entry = entry->next;
        }
    }
    free(position);
}

BRS_GUI_Menu *
BRS_GUI_Menu_create(BRS_GUI_MenuBar *menuBar, BRS_Dimension *dimension, const char *label, const BRS_Color *foreColor,
                    const BRS_Color *backColor,
                    const BRS_Color *selectedForeColor, BRS_Font *font, bool selected) {
    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->menuBar = menuBar;
    menu->dimension = BRS_copyDimension(dimension);
    menu->label = label;
    menu->font = font;
    menu->itemList = BRS_GUI_MenuItemList_create();
    menu->foreColor = foreColor;
    menu->backColor = backColor;
    menu->selectedForeColor = selectedForeColor;
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

BRS_Point *BRS_GUI_Menu_calcPosition(BRS_GUI_Menu *menu) {
    BRS_Point *pos = malloc(sizeof(BRS_Point));
    BRS_Point *menuBarPos = BRS_GUI_MenuBar_getPosition(menu->menuBar);
    int32_t menuIndex = BRS_GUI_MenuBar_getMenuIndex(menu->menuBar, menu);
    pos->x = menuBarPos->x + menu->dimension->width * menuIndex;
    pos->y = menuBarPos->y;
    return pos;
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