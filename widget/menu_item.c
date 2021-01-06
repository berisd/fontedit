//
// Created by bernd on 05.01.21.
//

#include "menu_item.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(const char *label, const BRS_Color *foreColor, const BRS_Color *backColor, BRS_Font *font) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    menuItem->label = label;
    menuItem->foreColor = foreColor;
    menuItem->backColor = backColor;
    menuItem->font = font;
    return menuItem;
}

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem, BRS_Point position) {
    BRS_setColor(context, menuItem->backColor);
    SDL_Rect r = {.x = position.x, .y = position.y, .w=50, .h=18};
    SDL_RenderFillRect(context->renderer, &r);

    BRS_drawString(context, menuItem->label, menuItem->font, &position, menuItem->foreColor);
}

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler) {
    menuItem->clickHandler = handler;
}
