//
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
#include "widget.h"
#include "widgets.h"

BRS_LIST_DEFN(BRS_GUI_WidgetList, BRS_GUI_Widget)

static BRS_GUI_Widget *
createWidget(BRS_GUI_WidgetType type, BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme, void *object) {
    BRS_GUI_Widget *widget = malloc(sizeof(BRS_GUI_Widget));
    widget->type = type;
    widget->position = BRS_copyPoint(position);
    widget->size = BRS_copySize(size);
    widget->object = object;
    widget->theme = theme;
    widget->children = BRS_GUI_WidgetList_create();
    return widget;
}

BRS_GUI_Widget *
BRS_GUI_Widget_createWindow(BRS_Point *position, BRS_Size *size, BRS_GUI_Theme *theme) {
    BRS_GUI_Window *window = BRS_GUI_Window_create();
    return createWidget(BRS_GUI_WIDGET_WINDOW, position, size, theme, window);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text) {
    BRS_Size size = {0, 0};
    return createWidget(BRS_GUI_WIDGET_LABEL, position, &size, theme, BRS_GUI_Label_create(text));
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMessageBox(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme,
                                const char *title, const char *text) {
    return createWidget(BRS_GUI_WIDGET_MESSAGEBOX, position, size, theme, BRS_GUI_MessageBox_create(title, text));
}

BRS_GUI_Widget *
BRS_GUI_Widget_createInputBox(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme,
                              const char *title, const char *textLabel) {
    return createWidget(BRS_GUI_WIDGET_INPUTBOX, position, size, theme, BRS_GUI_InputBox_create(title, textLabel));
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme) {
    return createWidget(BRS_GUI_WIDGET_MENUBAR, position, size, theme, BRS_GUI_MenuBar_create());
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited) {
    BRS_Size size = {0, 0};
    return createWidget(BRS_GUI_WIDGET_CHARTABLE, position, &size, theme, BRS_GUI_CharTable_create(fontEdited));
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited) {
    BRS_Size size = {0, 0};
    return createWidget(BRS_GUI_WIDGET_CHAREDIT, position, &size, theme, BRS_GUI_CharEdit_create(fontEdited));
}

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_LABEL:
            BRS_GUI_Label_render(context, widget);
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
    BRS_GUI_WidgetList_destroy(widget->children);
    free(widget->size);
    free(widget->position);
    free(widget);
}

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, void *handler) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            ((BRS_GUI_MenuBar *) widget->object)->clickHandler = handler;
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            ((BRS_GUI_CharTable *) widget->object)->clickHandler = handler;
            break;
    }
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
