//
// Created by bernd on 05.01.21.
//

#include "menu_item.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

static void calculatePosition(BRS_GUI_MenuItem *menuItem, BRS_Point *menuItemPosition) {
    int32_t menuItemIndex = BRS_GUI_Menu_getMenuItemIndex(menuItem->menu, menuItem);
    BRS_Point menuPosition;
    BRS_GUI_Menu_calcPosition(menuItem->menu, &menuPosition);
    BRS_Dimension *menuDimension = BRS_GUI_Menu_getDimension(menuItem->menu);

    menuItemPosition->x = menuPosition.x;
    menuItemPosition->y = menuPosition.y + menuDimension->height + menuItem->dimension->height * menuItemIndex;
}

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_Dimension *dimension, BRS_GUI_Menu *menu, const char *label, const BRS_Color *foreColor,
                        const BRS_Color *backColor, const BRS_Color *highlightedColor, BRS_Font *font) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    menuItem->dimension = BRS_copyDimension(dimension);
    menuItem->menu = menu;
    menuItem->label = label;
    menuItem->foreColor = foreColor;
    menuItem->backColor = backColor;
    menuItem->highlightedColor = highlightedColor;
    menuItem->font = font;
    menuItem->clickHandler = NULL;
    menuItem->highlighted = false;
    return menuItem;
}

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem) {
    free(menuItem->dimension);
    free(menuItem);
}

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem) {
    BRS_Point position;
    calculatePosition(menuItem, &position);

    BRS_setColor(context, menuItem->backColor);
    BRS_Rect menuItemRect = {.x = position.x, .y = position.y, .width=menuItem->dimension->width, .height=menuItem->dimension->height};
    BRS_drawlFillRect(context, &menuItemRect);

    BRS_drawString(context, menuItem->label, menuItem->font, &position,
                   menuItem->highlighted ? menuItem->highlightedColor : menuItem->foreColor);
}

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler) {
    menuItem->clickHandler = handler;
}

static void processMouseButtonDown(BRS_GUI_MenuItem *menuItem, SDL_MouseButtonEvent *button) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Point menuPosition;
        BRS_GUI_Menu_calcPosition(menuItem->menu, &menuPosition);
        BRS_Point menuItemPosition;
        calculatePosition(menuItem, &menuItemPosition);

        BRS_Rect menuItemRect = {.x = menuItemPosition.x, .y = menuItemPosition.y, .width = menuItem->dimension->width,
                menuItem->dimension->height};
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        bool clickedInMenuItem = BRS_PointInRect(&mousePoint, &menuItemRect);

        if (clickedInMenuItem && menuItem->clickHandler) {
            menuItem->clickHandler(menuItem);
        }
    }
}

static void processMouseMove(BRS_GUI_MenuItem *menuItem, SDL_MouseMotionEvent *motion) {
    BRS_Point mousePoint = {.x = motion->x, .y = motion->y};
    BRS_Point position;
    calculatePosition(menuItem, &position);
    BRS_Rect widgetRect = {.x = position.x, .y = position.y, .width = menuItem->dimension->width, .height = menuItem->dimension->height};
    menuItem->highlighted = BRS_PointInRect(&mousePoint, &widgetRect);
}

void BRS_GUI_MenuItem_processEvent(BRS_GUI_MenuItem *menuItem, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            processMouseMove(menuItem, &event->motion);
            break;
        case SDL_MOUSEBUTTONUP:
            processMouseButtonDown(menuItem, &event->button);
            break;
    }
}