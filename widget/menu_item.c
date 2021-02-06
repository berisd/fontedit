//
// Created by bernd on 05.01.21.
//

#include "menu_item.h"
#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *
BRS_GUI_MenuItem_create(BRS_Size *size, BRS_Padding *padding, const char *label) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    menuItem->size = BRS_copySize(size);
    menuItem->label = label;
    menuItem->clickHandler = NULL;
    menuItem->highlighted = false;
    menuItem->position = malloc(sizeof(BRS_Point));
    menuItem->padding = BRS_copyPadding(padding);
    return menuItem;
}

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem) {
    free(menuItem->size);
    free(menuItem->position);
    free(menuItem->padding);
    free(menuItem);
}

void BRS_GUI_setMenuItemClickHandler(BRS_GUI_MenuItem *menuItem, BRS_GUI_MenuItem_ClickHandler handler) {
    menuItem->clickHandler = handler;
}

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_MenuItem *menuItem, BRS_GUI_Theme *theme) {
    BRS_Point *position = menuItem->position;
    BRS_setColor(context, theme->menuItemBackColor);
    BRS_Rect menuItemRect = {.x = position->x, .y = position->y, .width=menuItem->size->width, .height=menuItem->size->height};
    BRS_drawlFillRect(context, &menuItemRect);

    BRS_Point labelPosition = {.x = position->x + menuItem->padding->left, .y = position->y};
    BRS_setColor(context, menuItem->highlighted ? theme->menuItemHighlightedColor : theme->menuItemForeColor);
    BRS_drawString(context, menuItem->label, strlen(menuItem->label), theme->font, &labelPosition);
}

static bool
processMenuItemMouseButtonDown(BRS_GUI_MenuItem *menuItem, SDL_MouseButtonEvent *button, BRS_GUI_Widget *menuWidget) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Rect menuItemRect = {.x = menuItem->position->x, .y = menuItem->position->y, .width = menuItem->size->width,
                menuItem->size->height};
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        bool clickedInMenuItem = BRS_PointInRect(&mousePoint, &menuItemRect);

        if (clickedInMenuItem && menuItem->clickHandler) {
            if (menuWidget->properties->visible) {
                BRS_GUI_Menu_hide(menuWidget);
            }
            menuItem->clickHandler(menuItem);
            return true;
        }
    }
    return false;
}

static bool processMenuItemMouseMove(BRS_GUI_MenuItem *menuItem, SDL_MouseMotionEvent *motion) {
    BRS_Point mousePoint = {.x = motion->x, .y = motion->y};
    BRS_Rect widgetRect = {.x = menuItem->position->x, .y = menuItem->position->y, .width = menuItem->size->width, .height = menuItem->size->height};
    bool inRect = BRS_PointInRect(&mousePoint, &widgetRect);
    menuItem->highlighted = inRect;
    return inRect ? true : false;
}

bool BRS_GUI_MenuItem_processEvent(BRS_GUI_MenuItem *menuItem, SDL_Event *event, BRS_GUI_Widget *menuWidget) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            return processMenuItemMouseMove(menuItem, &event->motion);
        case SDL_MOUSEBUTTONUP:
            return processMenuItemMouseButtonDown(menuItem, &event->button, menuWidget);
    }
}
