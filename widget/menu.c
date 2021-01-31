//
// Created by bernd on 05.01.21.
//

#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

BRS_GUI_Menu *
BRS_GUI_Menu_create(BRS_Size *size, const char *label, bool selected) {
    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->size = BRS_copySize(size);
    menu->label = label;
    menu->itemList = BRS_GUI_MenuItemList_create();
    menu->selected = selected;
    menu->itemList = BRS_GUI_MenuItemList_create();
    menu->position = malloc(sizeof(BRS_Point));
    return menu;
}

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu) {
    BRS_GUI_MenuItemListEntry *entry = menu->itemList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_MenuItem *menuItem = entry->value;
        BRS_GUI_MenuItem_destroy(menuItem);
        entry = entry->next;
    }
    free(menu->size);
    free(menu->position);
    free(menu);
}
