//
// Created by bernd on 02.01.21.
//
#include "widget.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuBar *BRS_GUI_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color) {
    BRS_GUI_MenuBar *menubar = malloc(sizeof(BRS_GUI_MenuBar));
    menubar->color = color;
    menubar->dimension = dimension;
    menubar->position = position;
    menubar->menuList = BRS_GUI_MenuList_create();
    return menubar;
}

BRS_GUI_Menu *BRS_GUI_renderMenuBar(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar, BRS_Font *font) {
    BRS_setColor(context, menubar->color);
    SDL_Rect r = {.x = menubar->position->x, .y = menubar->position->y, .w=menubar->dimension->width, .h=menubar->dimension->height};
    SDL_RenderFillRect(context->renderer, &r);

    BRS_GUI_Menu *fileMenu = menubar->menuList->firstEntry->value;
    BRS_Point filemenuPos = {.x = menubar->position->x + 1, .y = menubar->position->y + 1};
    BRS_drawString(context, fileMenu->label, font, &filemenuPos, &COLOR_WHITE);
}
