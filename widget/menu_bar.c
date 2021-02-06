//
// Created by bernd on 05.01.21.
//

#include "menu_bar.h"

static void BRS_GUI_MenuBar_hideMenu(BRS_GUI_Widget *menuBarWidget, BRS_GUI_Widget *menuWidget) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    if (menuWidgetProps->visible) {
        menuBarWidget->properties->zIndex--;
        menuWidgetProps->zIndex--;
        menuWidgetProps->visible = false;
    }
}

static void BRS_GUI_MenuBar_showMenu(BRS_GUI_Widget *menuBarWidget, BRS_GUI_Widget *menuWidget) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    if (!menuWidgetProps->visible) {
        menuBarWidget->properties->zIndex++;
        menuWidgetProps->zIndex++;
        menuWidgetProps->visible = true;
    }
}

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create() {
    BRS_GUI_MenuBar *menuBar = malloc(sizeof(BRS_GUI_MenuBar));
    return menuBar;
}

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar) {
    free(menuBar);
}

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_setColor(context, widgetProps->theme->menuBarColor);
    BRS_Rect r = {.x = widgetProps->position->x, .y = widgetProps->position->y, .width=widgetProps->size->width, .height=widgetProps->size->height};
    BRS_drawlFillRect(context, &r);

    BRS_GUI_WidgetListEntry *entry = widget->children->firstEntry;
    BRS_Point menuPosition = {.x = widgetProps->position->x, .y = widgetProps->position->y};
    BRS_Point menuLabelPosition = {.x = menuPosition.x, .y = menuPosition.y};
    while (entry != NULL) {
        BRS_GUI_Widget *menuWidget = entry->value;
        BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
        BRS_GUI_Menu *menu = menuWidget->object;
        BRS_setColor(context,
                     menuWidgetProps->visible ? menuWidgetProps->theme->menuSelectedForeColor
                                              : menuWidgetProps->theme->menuForeColor);

        menuLabelPosition.x = menuPosition.x + menuWidgetProps->padding->left;

        BRS_drawString(context, menu->label, strlen(menu->label), menuWidgetProps->theme->font, &menuLabelPosition);
        menuPosition.x += menuWidgetProps->size->width;
        entry = entry->next;
    }
}

static bool BRS_GUI_MenuBar_processMouseButtonDown(BRS_GUI_Widget *widget, SDL_MouseButtonEvent *button) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_GUI_Widget_Properties *widgetProps = widget->properties;
        BRS_GUI_Widget *firstMenuWidget =
                widget->children->firstEntry != NULL ? widget->children->firstEntry->value : NULL;
        BRS_GUI_Widget_Properties *firstMenuWidgetProps = firstMenuWidget->properties;

        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        BRS_Rect menuBarRect = {.x = widgetProps->position->x, .y = widgetProps->position->y,
                .width = widgetProps->size->width, .height = widgetProps->size->height};
        BRS_Rect firstMenuRect = {.x = firstMenuWidgetProps->position->x, .y = firstMenuWidgetProps->position->y,
                .width = firstMenuWidgetProps->size->width, .height = firstMenuWidgetProps->size->height};

        bool isMouseInMenuBarRect = BRS_PointInRect(&mousePoint, &menuBarRect);
        bool isMouseInFirstMenuRect = BRS_PointInRect(&mousePoint, &firstMenuRect);

        if (firstMenuWidgetProps->visible) {
            if (!isMouseInFirstMenuRect) {
                BRS_GUI_MenuBar_hideMenu(widget, firstMenuWidget);
                return true;
            }
        }

        if (isMouseInMenuBarRect) {
            if (firstMenuWidget->properties->visible) {
                BRS_GUI_MenuBar_hideMenu(widget, firstMenuWidget);
            } else {
                BRS_GUI_MenuBar_showMenu(widget, firstMenuWidget);
            }
            return true;
        }
    }
    return false;
}

bool BRS_GUI_MenuBar_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            return BRS_GUI_MenuBar_processMouseButtonDown(widget, &event->button);
    }

    return false;
}

void BRS_GUI_MenuBar_executeAction(BRS_GUI_Widget *widget, const char *action) {
    if (strcmp(BRS_GUI_MENU_ACTION_HIDE, action) == 0) {
        BRS_GUI_Widget *firstMenuWidget =
                widget->children->firstEntry != NULL ? widget->children->firstEntry->value : NULL;
        BRS_GUI_MenuBar_hideMenu(widget, firstMenuWidget);
    }
}
