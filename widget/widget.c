//
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
#include "widget.h"

BRS_LIST_DEFN(BRS_GUI_WidgetList, BRS_GUI_Widget)

static BRS_GUI_Widget *createWidget(BRS_GUI_WidgetType type, void *object) {
    BRS_GUI_Widget *widget = malloc(sizeof(BRS_GUI_Widget));
    widget->type = type;
    widget->object = malloc(sizeof(BRS_GUI_Widget_Object));
    widget->children = BRS_GUI_WidgetList_create();

    switch (type) {
        case BRS_GUI_WIDGET_WINDOW:
            widget->object->window = object;
            break;
        case BRS_GUI_WIDGET_LABEL:
            widget->object->label = object;
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            widget->object->menuBar = object;
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            widget->object->charTable = object;
            break;
        case BRS_GUI_WIDGET_CHAREDIT:
            widget->object->charEdit = object;
            break;
        case BRS_GUI_WIDGET_MESSAGEBOX:
            widget->object->messageBox = object;
            break;
        case BRS_GUI_WIDGET_INPUTBOX:
            widget->object->inputBox = object;
            break;
    }

    return widget;
}

BRS_GUI_Widget *
BRS_GUI_Widget_createWindow() {
    BRS_GUI_Window *window = BRS_GUI_Window_create();
    return createWidget(BRS_GUI_WIDGET_WINDOW, window);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text) {
    BRS_GUI_Label *label = BRS_GUI_Label_create(position, theme, text);
    return createWidget(BRS_GUI_WIDGET_LABEL, label);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMessageBox(BRS_Point *position, struct BRS_Dimension *dimension, const BRS_GUI_Theme *theme,
                                const char *title, const char *text) {
    BRS_GUI_MessageBox *messageBox = BRS_GUI_MessageBox_create(position, dimension, theme, title, text);
    return createWidget(BRS_GUI_WIDGET_MESSAGEBOX, messageBox);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createInputBox(BRS_Point *position, struct BRS_Dimension *dimension, const BRS_GUI_Theme *theme,
                              const char *title, const char *textLabel) {
    BRS_GUI_InputBox *inputBox = BRS_GUI_InputBox_create(position, dimension, theme, title, textLabel);
    return createWidget(BRS_GUI_WIDGET_INPUTBOX, inputBox);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_GUI_Theme *theme) {
    BRS_GUI_MenuBar *menubar = BRS_GUI_MenuBar_create(position, dimension, theme);
    return createWidget(BRS_GUI_WIDGET_MENUBAR, menubar);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_create(position, theme, fontEdited);
    return createWidget(BRS_GUI_WIDGET_CHARTABLE, charTable);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited) {
    BRS_GUI_CharEdit *charEdit = BRS_GUI_CharEdit_create(position, theme, fontEdited);
    return createWidget(BRS_GUI_WIDGET_CHAREDIT, charEdit);
}

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_LABEL:
            BRS_GUI_Label_render(context, widget->object->label);
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_MenuBar_render(context, widget->object->menuBar);
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            BRS_GUI_CharTable_render(context, widget->object->charTable);
            break;
        case BRS_GUI_WIDGET_CHAREDIT:
            BRS_GUI_CharEdit_render(context, widget->object->charEdit);
            break;
        case BRS_GUI_WIDGET_MESSAGEBOX:
            BRS_GUI_MessageBox_render(context, widget->object->messageBox);
            break;
        case BRS_GUI_WIDGET_INPUTBOX:
            BRS_GUI_InputBox_render(context, widget->object->inputBox);
            break;
    }
}

void BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_MenuBar_processEvent(widget->object->menuBar, event);
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            BRS_GUI_CharTable_processEvent(widget->object->charTable, event);
            break;
        case BRS_GUI_WIDGET_CHAREDIT:
            BRS_GUI_CharEdit_processEvent(widget->object->charEdit, event);
            break;
        case BRS_GUI_WIDGET_MESSAGEBOX:
            BRS_GUI_MessageBox_processEvent(widget->object->messageBox, event);
            break;
        case BRS_GUI_WIDGET_INPUTBOX:
            BRS_GUI_InputBox_processEvent(widget->object->inputBox, event);
            break;
    }
}

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_WINDOW:
            BRS_GUI_Window_destroy(widget->object->window);
            break;
        case BRS_GUI_WIDGET_LABEL:
            BRS_GUI_Label_destroy(widget->object->label);
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_MenuBar_destroy(widget->object->menuBar);
            break;
        case BRS_GUI_WIDGET_CHARTABLE:
            BRS_GUI_CharTable_destroy(widget->object->charTable);
            break;
        case BRS_GUI_WIDGET_CHAREDIT:
            BRS_GUI_CharEdit_destroy(widget->object->charEdit);
            break;
        case BRS_GUI_WIDGET_MESSAGEBOX:
            BRS_GUI_MessageBox_destroy(widget->object->messageBox);
            break;
        case BRS_GUI_WIDGET_INPUTBOX:
            BRS_GUI_InputBox_destroy(widget->object->inputBox);
            break;
    }
    BRS_GUI_WidgetList_destroy(widget->children);
    free(widget->object);
    free(widget);
}

void BRS_GUI_Widget_addChild(BRS_GUI_Widget *widget, BRS_GUI_Widget *child) {
    BRS_GUI_WidgetList_push(child, widget->children);
}

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, void *handler) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            widget->object->menuBar->clickHandler = handler;
            break;
    }
}
