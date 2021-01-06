//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_ITEM_H
#define BRS_GUI_MENU_ITEM_H

#include "../list.h"
#include "../video.h"

typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

typedef void (*BRS_GUI_MenuItem_ClickHandler)(BRS_GUI_MenuItem *);

struct _BRS_GUI_MenuItem {
    const char *label;
    const BRS_Color *foreColor;
    const BRS_Color *backColor;
    BRS_Font *font;
    BRS_GUI_MenuItem_ClickHandler clickHandler;
};

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *BRS_GUI_MenuItem_create(const char *label, const BRS_Color *foreColor, const BRS_Color *backColor, BRS_Font *font);

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem, BRS_Point position);

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler);

#endif //BRS_GUI_MENU_ITEM_H
