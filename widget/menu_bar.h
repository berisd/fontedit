//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_MENU_BAR_H
#define BRS_GUI_MENU_BAR_H

#include "menu.h"

typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;
typedef void (*BRS_GUI_MenuBar_ClickHandler)(BRS_GUI_MenuBar *);

struct _BRS_GUI_MenuBar {
    BRS_Font *font;
    BRS_Point *position;
    BRS_Dimension *dimension;
    const BRS_Color *color;
    BRS_GUI_MenuList *menuList;
    BRS_GUI_MenuBar_ClickHandler clickHandler;
};

BRS_GUI_MenuBar *BRS_GUI_MenuBar_create(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color, BRS_Font *font);

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar);

#endif //BRS_GUI_MENU_BAR_H
