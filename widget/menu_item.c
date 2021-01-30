//
// Created by bernd on 05.01.21.
//

#include "menu_item.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_Size *size, const char *label) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    menuItem->size = BRS_copySize(size);
    menuItem->label = label;
    menuItem->clickHandler = NULL;
    menuItem->highlighted = false;
    return menuItem;
}

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem) {
    free(menuItem->size);
    free(menuItem);
}

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler) {
    menuItem->clickHandler = handler;
}
