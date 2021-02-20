//
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
#include "widget.h"
#include "widgets.h"

BRS_LIST_DEFN(BRS_GUI_WidgetList, BRS_GUI_Widget)

static BRS_GUI_Widget *
createWidget(BRS_GUI_WidgetType type, BRS_GUI_Widget_Properties *properties, void *object, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget *widget = malloc(sizeof(BRS_GUI_Widget));
    widget->type = type;
    widget->properties = properties;
    widget->object = object;
    widget->children = BRS_GUI_WidgetList_create();
    widget->parent = parent;
    widget->clickHandler = NULL;
    return widget;
}

BRS_GUI_Widget *
BRS_GUI_Widget_createWindow(BRS_GUI_Widget_Properties *properties) {
    BRS_GUI_Window *window = BRS_GUI_Window_create();
    return createWidget(BRS_GUI_WIDGET_WINDOW, properties, window, NULL);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_GUI_Widget_Properties *properties, const char *text, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_LABEL, properties, BRS_GUI_Label_create(text), parent);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMessageBox(BRS_GUI_Widget_Properties *properties, const char *title, const char *text, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_MESSAGEBOX, properties, BRS_GUI_MessageBox_create(title, text), parent);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createInputBox(BRS_GUI_Widget_Properties *properties, const char *title, const char *textLabel, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_INPUTBOX, properties, BRS_GUI_InputBox_create(title, textLabel), parent);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_GUI_Widget_Properties *properties, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_MENUBAR, properties, BRS_GUI_MenuBar_create(), parent);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMenu(BRS_GUI_Widget_Properties *properties, void *menu, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_MENU, properties, menu, parent);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuItem(BRS_GUI_Widget_Properties *properties, void *menuItem, BRS_GUI_Widget *parentWidget) {
    BRS_GUI_Widget *menuItemWidget = createWidget(BRS_GUI_WIDGET_MENU_ITEM, properties, menuItem, parentWidget);
    BRS_GUI_WidgetList_push(menuItemWidget, parentWidget->children);
    return menuItemWidget;
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_GUI_Widget_Properties *properties, BRS_Font *fontEdited, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_CHARTABLE, properties, BRS_GUI_CharTable_create(fontEdited), parent);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_GUI_Widget_Properties *properties, BRS_Font *fontEdited, BRS_GUI_Widget *parent) {
    return createWidget(BRS_GUI_WIDGET_CHAREDIT, properties, BRS_GUI_CharEdit_create(fontEdited), parent);
}

void BRS_GUI_Widget_calculate(BRS_GUI_Widget *widget, BRS_GUI_Widget *parentWidget, int32_t widgetIndex) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENU:
            BRS_GUI_Menu_calculate(widget, parentWidget, widgetIndex);
            break;
    }
}

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_LABEL:
            BRS_GUI_Label_render(context, widget);
            break;
        case BRS_GUI_WIDGET_MENU:
            BRS_GUI_Menu_render(context, widget);
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_MenuBar_render(context, widget);
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            BRS_GUI_CharTable_render(context, widget);
            break;
        case BRS_GUI_WIDGET_CHAREDIT:
            BRS_GUI_CharEdit_render(context, widget);
            break;
        case BRS_GUI_WIDGET_MESSAGEBOX:
            BRS_GUI_MessageBox_render(context, widget);
            break;
        case BRS_GUI_WIDGET_INPUTBOX:
            BRS_GUI_InputBox_render(context, widget);
            break;
    }
}

bool BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            return BRS_GUI_MenuBar_processEvent(widget, event);
        case BRS_GUI_WIDGET_MENU:
            return BRS_GUI_Menu_processEvent(widget, event);
        case BRS_GUI_WIDGET_CHARTABLE:
            return BRS_GUI_CharTable_processEvent(widget, event);
        case BRS_GUI_WIDGET_CHAREDIT:
            return BRS_GUI_CharEdit_processEvent(widget, event);
        case BRS_GUI_WIDGET_MESSAGEBOX:
            return BRS_GUI_MessageBox_processEvent(widget, event);
        case BRS_GUI_WIDGET_INPUTBOX:
            return BRS_GUI_InputBox_processEvent(widget, event);
        default:
            return false;
    }
}

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_WINDOW:
            BRS_GUI_Window_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_LABEL:
            BRS_GUI_Label_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_MenuBar_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_MENU:
            BRS_GUI_Menu_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            BRS_GUI_CharTable_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_CHAREDIT:
            BRS_GUI_CharEdit_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_MESSAGEBOX:
            BRS_GUI_MessageBox_destroy(widget->object);
            break;
        case BRS_GUI_WIDGET_INPUTBOX:
            BRS_GUI_InputBox_destroy(widget->object);
            break;
    }

    BRS_GUI_WidgetListEntry *listEntry = widget->children->firstEntry;
    while (listEntry != NULL) {
        BRS_GUI_Widget *widget = listEntry->value;
        BRS_GUI_Widget_destroy(widget);
        listEntry = listEntry->next;
    }

    BRS_GUI_WidgetList_destroy(widget->children);
    BRS_GUI_Widget_Properties_destroy(widget->properties);
    free(widget);
}

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, BRS_GUI_Widget_ClickHandler handler) {
    widget->clickHandler = handler;
}

BRS_GUI_Widget *BRS_GUI_Widget_getByType(BRS_GUI_WidgetType type, BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->type == type) {
            return widget;
        }
        BRS_GUI_Widget *widgetFound = BRS_GUI_Widget_getByType(type, widget->children);
        if (widgetFound != NULL) {
            return widgetFound;
        }
        entry = entry->next;
    }
    return NULL;
}

BRS_GUI_Widget *BRS_GUI_Widget_getByData(void *data, BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->object == data) {
            return widget;
        }
        BRS_GUI_Widget *widgetFound = BRS_GUI_Widget_getByData(data, widget->children);
        if (widgetFound != NULL) {
            return widgetFound;
        }
        entry = entry->next;
    }
    return NULL;
}

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_create() {
    BRS_GUI_Widget_Properties *properties = malloc(sizeof(BRS_GUI_Widget_Properties));

    BRS_Point position = {.x = 0, .y = 0};
    properties->position = BRS_copyPoint(&position);

    BRS_Size size = {.width = 0, .height = 0};
    properties->size = BRS_copySize(&size);

    BRS_Padding padding = {.left = 0, .right = 0, .top = 0, .bottom = 0};
    properties->padding = BRS_copyPadding(&padding);

    properties->theme = NULL;
    properties->zIndex = 0;
    properties->visible = false;

    return properties;
}

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_copy(BRS_GUI_Widget_Properties* source) {
    BRS_GUI_Widget_Properties *properties = malloc(sizeof(BRS_GUI_Widget_Properties));

    properties->position = BRS_copyPoint(source->position);
    properties->size = BRS_copySize(source->size);
    properties->padding = BRS_copyPadding(source->padding);

    properties->theme = source->theme;
    properties->zIndex = source->zIndex;
    properties->visible = source->visible;

    return properties;
}

void BRS_GUI_Widget_Properties_destroy(BRS_GUI_Widget_Properties *properties) {
    free(properties->padding);
    free(properties->position);
    free(properties->size);
    free(properties);
}

void BRS_GUI_Widget_executeAction(BRS_GUI_Widget *widget, const char *action) {
    if (widget == NULL || action == NULL) {
        return;
    }
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_MenuBar_executeAction(widget, action);
            break;
    }
}

BRS_GUI_Widget *BRS_GUI_Widget_findRootWidget(BRS_GUI_Widget *widget) {
    if (widget == NULL) {
        return NULL;
    }
    BRS_GUI_Widget *currentWidget = widget;
    while (currentWidget->parent != NULL) {
        currentWidget = currentWidget->parent;
    }
    return currentWidget;
}

static BRS_GUI_Widget *BRS_GUI_Widget_getByTypeFromList(BRS_GUI_WidgetType type, BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->type == type) {
            return widget;
        }
        BRS_GUI_Widget *widgetFound = BRS_GUI_Widget_getByTypeFromList(type, widget->children);
        if (widgetFound != NULL) {
            return widgetFound;
        }
        entry = entry->next;
    }
    return NULL;
}

BRS_GUI_Widget *BRS_GUI_Widget_findWidgetByType(BRS_GUI_WidgetType widgetType, BRS_GUI_Widget *rootWidget) {
    if (rootWidget == NULL) {
        return NULL;
    }

    if (rootWidget->type == widgetType) {
        return rootWidget;
    } else {
        return BRS_GUI_Widget_getByTypeFromList(widgetType, rootWidget->children);
    }
}
