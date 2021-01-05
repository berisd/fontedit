//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_H
#define BRS_GUI_MENU_H

#include <stdbool.h>
#include "../video.h"
#include "menu_item.h"

typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

struct _BRS_GUI_Menu {
    const char *label;
    const BRS_Color *foreColor;
    const BRS_Color *backColor;
    const BRS_Color *selectedForeColor;
    BRS_Font *font;
    bool selected;
    BRS_GUI_MenuItemList *itemList;
};

BRS_LIST_DECL(BRS_GUI_MenuList, BRS_GUI_Menu)

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Menu *menu, BRS_Point position);

BRS_GUI_Menu *
BRS_GUI_Menu_create(const char *label, const BRS_Color *foreColor, const BRS_Color *backColor, const BRS_Color *selectedForeColor, BRS_Font *font, bool selected);

#endif //BRS_GUI_MENU_H
