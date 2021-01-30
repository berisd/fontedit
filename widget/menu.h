//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_H
#define BRS_GUI_MENU_H

#include <stdbool.h>
#include "../video.h"
#include "../theme.h"
#include "menu_item.h"

typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

struct _BRS_GUI_Menu {
    BRS_Size *size;
    const char *label;
    bool selected;
    BRS_GUI_MenuItemList *itemList;
};

BRS_LIST_DECL(BRS_GUI_MenuList, BRS_GUI_Menu)

BRS_GUI_Menu *
BRS_GUI_Menu_create(BRS_Size *size, const char *label, bool selected);

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu);

#endif //BRS_GUI_MENU_H
