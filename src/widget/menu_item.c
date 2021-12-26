//
// Created by bernd on 05.01.21.
//

#include "string.h"
#include "menu_item.h"
#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

static void BRS_GUI_MenuItem_render(const BRS_GUI_Widget *widget, const BRS_VideoContext *context) {
    BRS_GUI_MenuItem *menuItem = (BRS_GUI_MenuItem *) widget;
    BRS_GUI_Widget_Properties *menuItemProps = widget->properties;

    if (!menuItemProps->visible) {
        return;
    }

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

static bool processMenuItemMouseButtonDown(BRS_GUI_MenuItem *menuItem, SDL_MouseButtonEvent *button) {
    BRS_GUI_Menu *menu = (BRS_GUI_Menu *) menuItem->widget.properties->parent;
    BRS_GUI_Widget_Properties *menuItemProps = menuItem->widget.properties;
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Rect menuItemRect = {.x = menuItemProps->position->x, .y = menuItemProps->position->y, .width = menuItemProps->size->width,
                menuItemProps->size->height};
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        bool clickedInMenuItem = BRS_PointInRect(&mousePoint, &menuItemRect);

        if (clickedInMenuItem && menuItemProps->clickHandler != NULL) {
            if (menu->widget.properties->visible) {
                BRS_GUI_Menu_hide(menu);
            }
            menuItemProps->clickHandler((BRS_GUI_Widget *) menuItem);
            return true;
        }
    }
    return false;
}

static bool processMenuItemMouseMove(BRS_GUI_MenuItem *menuItem, SDL_MouseMotionEvent *motion) {
    BRS_GUI_Widget_Properties *widgetProps = menuItem->widget.properties;
    BRS_Point mousePoint = {.x = motion->x, .y = motion->y};
    BRS_Rect widgetRect = {.x = widgetProps->position->x, .y = widgetProps->position->y,
            .width = widgetProps->size->width, .height = widgetProps->size->height};
    bool inRect = BRS_PointInRect(&mousePoint, &widgetRect);
    BRS_GUI_WidgetListEntry *menuItemEntry = menuItem->widget.properties->parent->properties->children->firstEntry;
    while (menuItemEntry != NULL) {
        BRS_GUI_MenuItem *aMenuItem = (BRS_GUI_MenuItem *) menuItemEntry->value;
        aMenuItem->highlighted = aMenuItem == menuItem && inRect;
        menuItemEntry = menuItemEntry->next;
    }
    return inRect;
}

static bool BRS_GUI_MenuItem_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    if (!widget->properties->visible) {
        return false;
    }

    BRS_GUI_MenuItem *menuItem = (BRS_GUI_MenuItem *) widget;
    switch (event->type) {
        case SDL_MOUSEMOTION:
            return processMenuItemMouseMove(menuItem, &event->motion);
        case SDL_MOUSEBUTTONUP:
            return processMenuItemMouseButtonDown(menuItem, &event->button);
    }
}

void BRS_GUI_MenuItem_ctor(BRS_GUI_MenuItem *menuItem, BRS_GUI_Widget_Properties *widgetProps, const char *label) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) menuItem, widgetProps);
    menuItem->label = strdup(label);
    menuItem->highlighted = false;
    widgetProps->renderHandler = BRS_GUI_MenuItem_render;
    widgetProps->processEventHandler = BRS_GUI_MenuItem_processEvent;
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_MenuItem_destroy;
}

void BRS_GUI_MenuItem_dtor(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) menuItem);
    free(menuItem->label);
}

BRS_GUI_MenuItem *BRS_GUI_MenuItem_create(BRS_GUI_Widget_Properties *widgetProps, const char *label) {
    BRS_GUI_MenuItem *menuItem = malloc(sizeof(BRS_GUI_MenuItem));
    BRS_GUI_MenuItem_ctor(menuItem, widgetProps, label);
    return menuItem;
}

void BRS_GUI_MenuItem_destroy(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_MenuItem_dtor(menuItem);
    free(menuItem);
}
