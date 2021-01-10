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

    switch (type) {
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
    }

    return widget;
}

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_Point *position, const BRS_Color *color, const char *text, BRS_Font *font) {
    BRS_GUI_Label *label = BRS_GUI_Label_create(position, color, text, font);
    return createWidget(BRS_GUI_WIDGET_LABEL, label);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *foreColor,
                             BRS_Font *font) {
    BRS_GUI_MenuBar *menubar = BRS_GUI_MenuBar_create(position, dimension, foreColor, font);
    return createWidget(BRS_GUI_WIDGET_MENUBAR, menubar);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_Point *position, const BRS_Color *borderColor, const BRS_Color *charColor,
                               const BRS_Color *highlightedColor, const BRS_Color *selectedColor,
                               BRS_Font *fontEdited) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_create(position, borderColor, charColor, highlightedColor,
                                                            selectedColor, fontEdited);
    return createWidget(BRS_GUI_WIDGET_CHARTABLE, charTable);
}

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_Point *position, const BRS_Color *foreColor, const BRS_Color *dotColor,
                              const BRS_Color *clearColor, BRS_Font *fontEdited) {
    BRS_GUI_CharEdit *charEdit = BRS_GUI_CharEdit_create(position, foreColor, dotColor, clearColor, fontEdited);
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
    }
}

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget) {
    switch (widget->type) {
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
    }
    free(widget->object);
    free(widget);
}

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, void* handler) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            widget->object->menuBar->clickHandler = handler;
            break;
    }
}
