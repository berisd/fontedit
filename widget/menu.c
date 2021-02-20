//
// Created by bernd on 05.01.21.
//

#include "menu.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

const char *BRS_GUI_MENU_ACTION_HIDE = "CLOSE_MENU";

BRS_GUI_Menu *
BRS_GUI_Menu_create(const char *label) {
    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->label = label;
    return menu;
}

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu) {
    free(menu);
}

static void
BRS_GUI_Menu_calcPosition(BRS_GUI_Widget *menuWidget, BRS_GUI_Widget *parentWidget, int32_t menuIndex) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    BRS_GUI_Widget_Properties *parentWidgetProps = parentWidget->properties;
    menuWidgetProps->position->x =
            parentWidgetProps->position->x + menuWidgetProps->size->width * menuIndex;
    menuWidgetProps->position->y = parentWidgetProps->position->y + parentWidgetProps->size->height;
}

static void
BRS_GUI_Menu_calcSize(BRS_GUI_Widget *menuWidget) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    BRS_GUI_Widget *firstMenuItem = menuWidget->children->firstEntry->value;
    menuWidgetProps->size->height =
            menuWidget->children->size * (firstMenuItem != NULL ? firstMenuItem->properties->size->height : 0);
}

static void calculateMenuItemPosition(BRS_GUI_Widget *menuItem, BRS_GUI_Widget *menuWidget, int32_t menuItemIndex) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    menuItem->properties->position->x = menuWidgetProps->position->x;
    menuItem->properties->position->y =
            menuWidgetProps->position->y + menuItem->properties->size->height * menuItemIndex;
}

void BRS_GUI_Menu_calculate(BRS_GUI_Widget *menuWidget, BRS_GUI_Widget *parentWidget,
                            int32_t widgetIndex) {
    BRS_GUI_Menu_calcPosition(menuWidget, parentWidget, widgetIndex);
    BRS_GUI_Menu_calcSize(menuWidget);
    if (menuWidget->properties->visible) {
        int32_t menuItemIndex = 0;
        BRS_GUI_WidgetListEntry *entry = menuWidget->children->firstEntry;
        while (entry != NULL) {
            calculateMenuItemPosition(entry->value, menuWidget, menuItemIndex);
            entry = entry->next;
            menuItemIndex++;
        }
    }
}

void BRS_GUI_Menu_render(BRS_VideoContext *context, BRS_GUI_Widget *menuWidget) {
    if (!menuWidget->properties->visible) {
        return;
    }

    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    BRS_Point *position = menuWidgetProps->position;
    BRS_setColor(context, menuWidgetProps->theme->menuBackColor);
    BRS_Rect menuRect = {.x = position->x, .y = position->y, .width = menuWidgetProps->size->width,
            .height = menuWidgetProps->size->height};
    BRS_drawlFillRect(context, &menuRect);

    BRS_GUI_WidgetListEntry *entry = menuWidget->children->firstEntry;
    while (entry != NULL) {
        BRS_GUI_MenuItem_render(context, entry->value);
        entry = entry->next;
    }
}

bool BRS_GUI_Menu_processEvent(BRS_GUI_Widget *menuWidget, SDL_Event *event) {
    if (!menuWidget->properties->visible) {
        return false;
    }

    BRS_GUI_WidgetListEntry *menuItemEntry = menuWidget->children->firstEntry;
    while (menuItemEntry != NULL) {
        BRS_GUI_Widget *menuItem = menuItemEntry->value;
        if (BRS_GUI_MenuItem_processEvent(menuItem, event)) {
            return true;
        }
        menuItemEntry = menuItemEntry->next;
    }
    return false;
}

void BRS_GUI_Menu_hide(BRS_GUI_Widget *menuWidget) {
    if (menuWidget->parent) {
        BRS_GUI_Widget_executeAction(menuWidget->parent, BRS_GUI_MENU_ACTION_HIDE);
    } else {
        menuWidget->properties->visible = false;
    }
}
