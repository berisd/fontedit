//
// Created by bernd on 05.01.21.
//

#include "menu_item.h"
#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_GUI_MenuItem *BRS_GUI_MenuItem_create(const char *label) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    menuItem->label = label;
    menuItem->highlighted = false;
    return menuItem;
}

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem) {
    free(menuItem);
}

void BRS_GUI_MenuItem_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_MenuItem *menuItem = widget->object;
    BRS_GUI_Widget_Properties *menuItemProps = widget->properties;
    BRS_setColor(context, menuItemProps->theme->menuItemBackColor);
    BRS_Rect menuItemRect = {.x = menuItemProps->position->x, .y = menuItemProps->position->y,
            .width=menuItemProps->size->width, .height=menuItemProps->size->height};
    BRS_drawlFillRect(context, &menuItemRect);

    BRS_Point labelPosition = {.x = menuItemProps->position->x +
                                    menuItemProps->padding->left, .y = menuItemProps->position->y};
    BRS_setColor(context, menuItem->highlighted ? menuItemProps->theme->menuItemHighlightedColor
                                                : menuItemProps->theme->menuItemForeColor);
    BRS_drawString(context, menuItem->label, strlen(menuItem->label), menuItemProps->theme->font, &labelPosition);
}

static bool
processMenuItemMouseButtonDown(BRS_GUI_Widget *widget, SDL_MouseButtonEvent *button) {
    BRS_GUI_Widget *menuWidget = widget->parent;
    BRS_GUI_Widget_Properties *menuItemProps = widget->properties;
    BRS_GUI_MenuItem *menuItem = widget->object;
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Rect menuItemRect = {.x = menuItemProps->position->x, .y = menuItemProps->position->y, .width = menuItemProps->size->width,
                menuItemProps->size->height};
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        bool clickedInMenuItem = BRS_PointInRect(&mousePoint, &menuItemRect);

        if (clickedInMenuItem && widget->clickHandler) {
            if (menuWidget->properties->visible) {
                BRS_GUI_Menu_hide(menuWidget);
            }
            widget->clickHandler(widget);
            return true;
        }
    }
    return false;
}

static bool processMenuItemMouseMove(BRS_GUI_Widget *widget, SDL_MouseMotionEvent *motion) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_MenuItem *menuItem = widget->object;
    BRS_Point mousePoint = {.x = motion->x, .y = motion->y};
    BRS_Rect widgetRect = {.x = widgetProps->position->x, .y = widgetProps->position->y,
            .width = widgetProps->size->width, .height = widgetProps->size->height};
    bool inRect = BRS_PointInRect(&mousePoint, &widgetRect);
    menuItem->highlighted = inRect;
    BRS_GUI_WidgetListEntry *menuItemEntry = widget->parent->children->firstEntry;
    while (menuItemEntry != NULL) {
        BRS_GUI_Widget *menuItemWidget = menuItemEntry->value;
        BRS_GUI_MenuItem *menuItem = menuItemWidget->object;
        menuItem->highlighted = menuItemWidget == widget && inRect;
        menuItemEntry = menuItemEntry->next;
    }
    return inRect ? true : false;
}

bool BRS_GUI_MenuItem_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            return processMenuItemMouseMove(widget, &event->motion);
        case SDL_MOUSEBUTTONUP:
            return processMenuItemMouseButtonDown(widget, &event->button);
    }
}
