//
// Created by bernd on 05.01.21.
//

#include "menu.h"

const char *BRS_GUI_MENU_ACTION_HIDE = "CLOSE_MENU";

static void BRS_GUI_Menu_render(const BRS_GUI_Widget *menuWidget, const BRS_VideoContext *context) {
    if (!menuWidget->properties->visible) {
        return;
    }

    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    BRS_Point *position = menuWidgetProps->position;
    BRS_setColor(context, menuWidgetProps->theme->menuBackColor);
    BRS_Rect menuRect = {.x = position->x, .y = position->y, .width = menuWidgetProps->size->width,
            .height = menuWidgetProps->size->height};
    BRS_drawlFillRect(context, &menuRect);
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
    BRS_GUI_Widget *firstMenuItem = menuWidgetProps->children->firstEntry->value;
    menuWidgetProps->size->height =
            menuWidgetProps->children->size * (firstMenuItem != NULL ? firstMenuItem->properties->size->height : 0);
}

static void calculateMenuItemPosition(BRS_GUI_Widget *menuItem, BRS_GUI_Widget *menuWidget, int32_t menuItemIndex) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    menuItem->properties->position->x = menuWidgetProps->position->x;
    menuItem->properties->position->y =
            menuWidgetProps->position->y + menuItem->properties->size->height * menuItemIndex;
}

static void BRS_GUI_Menu_calculate(BRS_GUI_Widget *menuWidget) {
    BRS_GUI_Widget *parentWidget = menuWidget->properties->parent;
    int32_t widgetIndex = -1;
    BRS_GUI_WidgetListEntry *menuEntry = menuWidget->properties->parent->properties->children->firstEntry;
    while (menuEntry != NULL) {
        if (menuEntry->value == menuWidget) {
            widgetIndex = menuEntry->index;
            break;
        }
        menuEntry = menuEntry->next;
    }

    BRS_GUI_Menu_calcPosition(menuWidget, parentWidget, widgetIndex);
    BRS_GUI_Menu_calcSize(menuWidget);
    if (menuWidget->properties->visible) {
        int32_t menuItemIndex = 0;
        BRS_GUI_WidgetListEntry *entry = menuWidget->properties->children->firstEntry;
        while (entry != NULL) {
            calculateMenuItemPosition(entry->value, menuWidget, menuItemIndex);
            entry = entry->next;
            menuItemIndex++;
        }
    }
}

void BRS_GUI_Menu_ctor(BRS_GUI_Menu *menu, BRS_GUI_Widget_Properties *widgetProps, const char *label) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) menu, widgetProps);
    widgetProps->calculateHandler = BRS_GUI_Menu_calculate;
    widgetProps->renderHandler = BRS_GUI_Menu_render;
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_Menu_destroy;
    menu->label = (char *) label;
}

void BRS_GUI_Menu_dtor(BRS_GUI_Menu *menu) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) menu);
}

BRS_GUI_Menu *BRS_GUI_Menu_create(BRS_GUI_Widget_Properties *widgetProps, const char *label) {
    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    BRS_GUI_Menu_ctor(menu, widgetProps, label);
    return menu;
}

void BRS_GUI_Menu_destroy(BRS_GUI_Menu *menu) {
    BRS_GUI_Menu_dtor(menu);
    free(menu);
}

void BRS_GUI_Menu_hide(BRS_GUI_Menu *menu) {
    BRS_GUI_Widget *parentWidget = menu->widget.properties->parent;
    if (parentWidget) {
        parentWidget->properties->executeActionHandler(parentWidget, BRS_GUI_MENU_ACTION_HIDE);
    } else {
        menu->widget.properties->visible = false;
    }
}


