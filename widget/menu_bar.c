//
// Created by bernd on 05.01.21.
//

#include "menu_bar.h"

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color, BRS_Font *font) {
    BRS_GUI_MenuBar *menubar = malloc(sizeof(BRS_GUI_MenuBar));
    menubar->color = color;
    menubar->dimension = dimension;
    menubar->menuList = BRS_GUI_MenuList_create();
    menubar->position = position;
    menubar->font = font;
    return menubar;
}

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar) {
    BRS_setColor(context, menubar->color);
    SDL_Rect r = {.x = menubar->position->x, .y = menubar->position->y, .w=menubar->dimension->width, .h=menubar->dimension->height};
    SDL_RenderFillRect(context->renderer, &r);

    BRS_GUI_MenuListEntry *entry = menubar->menuList->firstEntry;
    BRS_Point menuPos = {.x = menubar->position->x + 1, .y = menubar->position->y + 1};
    while(entry != NULL) {
        BRS_GUI_Menu *menu = entry->value;
        BRS_GUI_Menu_render(context, menu, menuPos);
        menuPos.y+=20;
        entry = entry->next;
    }
}
