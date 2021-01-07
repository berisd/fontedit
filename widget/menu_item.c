//
// Created by bernd on 05.01.21.
//

#include "menu_item.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

static BRS_Point *calculatePosition(BRS_GUI_MenuItem *menuItem) {
    int32_t menuItemIndex = BRS_GUI_Menu_getMenuItemIndex(menuItem->menu, menuItem);
    BRS_Point *menuPosition = BRS_GUI_Menu_calcPosition(menuItem->menu);
    BRS_Dimension *menuDimension = BRS_GUI_Menu_getDimension(menuItem->menu);

    BRS_Point *menuItemPosition = malloc(sizeof(BRS_Point));
    menuItemPosition->x = menuPosition->x;
    menuItemPosition->y = menuPosition->y + menuDimension->height + menuItem->dimension->height * menuItemIndex;
    free(menuPosition);
    return menuItemPosition;
}

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_Dimension *dimension, BRS_GUI_Menu *menu, const char *label, const BRS_Color *foreColor,
                        const BRS_Color *backColor,
                        BRS_Font *font) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    menuItem->dimension = BRS_copyDimension(dimension);
    menuItem->menu = menu;
    menuItem->label = label;
    menuItem->foreColor = foreColor;
    menuItem->backColor = backColor;
    menuItem->font = font;
    menuItem->clickHandler = NULL;
    return menuItem;
}

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem) {
    free(menuItem->dimension);
    free(menuItem);
}

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem) {
    BRS_Point *position = calculatePosition(menuItem);

    BRS_setColor(context, menuItem->backColor);
    SDL_Rect r = {.x = position->x, .y = position->y, .w=menuItem->dimension->width, .h=menuItem->dimension->height};
    SDL_RenderFillRect(context->renderer, &r);

    position->x++;
    position->y++;

    BRS_drawString(context, menuItem->label, menuItem->font, position, menuItem->foreColor);
    free(position);
}

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler) {
    menuItem->clickHandler = handler;
}

static void processMouseDown(BRS_GUI_MenuItem *menuItem, SDL_MouseButtonEvent *button) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Point *menuPosition = BRS_GUI_Menu_calcPosition(menuItem->menu);
        BRS_Point *menuItemPosition = calculatePosition(menuItem);

        bool clickedInMenuItem = BRS_GUI_WIDGET_PointInRect(button->x, button->y, menuItemPosition,
                                                            menuItem->dimension);

        if (clickedInMenuItem && menuItem->clickHandler) {
            menuItem->clickHandler(menuItem);
        }

        free(menuPosition);
        free(menuItemPosition);
    }
}

void BRS_GUI_MenuItem_processEvent(BRS_GUI_MenuItem *menuItem, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
            processMouseDown(menuItem, &event->button);
            break;
    }
}