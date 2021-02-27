//
// Created by bernd on 05.01.21.
//

#include "menu_bar.h"
#include "menu.h"

static void BRS_GUI_MenuBar_hideMenu(BRS_GUI_Widget *menuBarWidget, BRS_GUI_Widget *menuWidget) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    if (menuWidgetProps->visible) {
        menuBarWidget->properties->zIndex--;
        menuWidgetProps->zIndex--;
        menuWidgetProps->visible = false;
        BRS_GUI_WidgetListEntry *childListEntry = menuWidgetProps->children->firstEntry;
        while (childListEntry != NULL) {
            BRS_GUI_Widget *menuItem = childListEntry->value;
            menuItem->properties->visible = false;
            childListEntry = childListEntry->next;
        }
    }
}

static void BRS_GUI_MenuBar_showMenu(BRS_GUI_Widget *menuBarWidget, BRS_GUI_Widget *menuWidget) {
    BRS_GUI_Widget_Properties *menuWidgetProps = menuWidget->properties;
    if (!menuWidgetProps->visible) {
        menuBarWidget->properties->zIndex++;
        menuWidgetProps->zIndex++;
        menuWidgetProps->visible = true;
        BRS_GUI_WidgetListEntry *childListEntry = menuWidgetProps->children->firstEntry;
        while (childListEntry != NULL) {
            BRS_GUI_Widget *menuItem = childListEntry->value;
            menuItem->properties->visible = true;
            childListEntry = childListEntry->next;
        }
    }
}

static bool BRS_GUI_MenuBar_processMouseButtonUp(BRS_GUI_Widget *widget, SDL_MouseButtonEvent *button) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_GUI_Widget_Properties *widgetProps = widget->properties;
        BRS_GUI_Widget *firstMenuWidget =
                widgetProps->children->firstEntry != NULL ? widgetProps->children->firstEntry->value : NULL;
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

static bool BRS_GUI_MenuBar_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            return BRS_GUI_MenuBar_processMouseButtonUp(widget, &event->button);
    }

    return false;
}

static void BRS_GUI_MenuBar_render(const BRS_GUI_Widget *widget, const BRS_VideoContext *context) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_setColor(context, widgetProps->theme->menuBarColor);
    BRS_Rect r = {.x = widgetProps->position->x, .y = widgetProps->position->y, .width=widgetProps->size->width, .height=widgetProps->size->height};
    BRS_drawlFillRect(context, &r);

    BRS_GUI_WidgetListEntry *entry = widgetProps->children->firstEntry;
    BRS_Point menuPosition = {.x = widgetProps->position->x, .y = widgetProps->position->y};
    BRS_Point menuLabelPosition = {.x = menuPosition.x, .y = menuPosition.y};
    while (entry != NULL) {
        BRS_GUI_Menu *menu = (BRS_GUI_Menu *) entry->value;
        BRS_GUI_Widget_Properties *menuWidgetProps = menu->widget.properties;
        BRS_setColor(context,
                     menuWidgetProps->visible ? menuWidgetProps->theme->menuSelectedForeColor
                                              : menuWidgetProps->theme->menuForeColor);

        menuLabelPosition.x = menuPosition.x + menuWidgetProps->padding->left;

        BRS_drawString(context, menu->label, strlen(menu->label), menuWidgetProps->theme->font, &menuLabelPosition);
        menuPosition.x += menuWidgetProps->size->width;
        entry = entry->next;
    }
}

static void BRS_GUI_MenuBar_executeAction(BRS_GUI_Widget *widget, const char *action) {
    if (strcmp(BRS_GUI_MENU_ACTION_HIDE, action) == 0) {
        BRS_GUI_Widget_Properties *widgetProps = widget->properties;
        BRS_GUI_Widget *firstMenuWidget =
                widgetProps->children->firstEntry != NULL ? widgetProps->children->firstEntry->value : NULL;
        BRS_GUI_MenuBar_hideMenu(widget, firstMenuWidget);
    }
}

void BRS_GUI_MenuBar_ctor(BRS_GUI_MenuBar *menuBar, BRS_GUI_Widget_Properties *widgetProps) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) menuBar, widgetProps);
    widgetProps->executeActionHandler = BRS_GUI_MenuBar_executeAction;
    widgetProps->renderHandler = BRS_GUI_MenuBar_render;
    widgetProps->processEventHandler = BRS_GUI_MenuBar_processEvent;
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_MenuBar_destroy;
}

void BRS_GUI_MenuBar_dtor(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) menuBar);
}

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create(BRS_GUI_Widget_Properties *widgetProps) {
    BRS_GUI_MenuBar *menuBar = malloc(sizeof(BRS_GUI_MenuBar));
    BRS_GUI_MenuBar_ctor(menuBar, widgetProps);
    return menuBar;
}

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_MenuBar_dtor(menuBar);
    free(menuBar);
}
