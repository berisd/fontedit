//
// Created by bernd on 02.01.21.
//
#include "widget.h"

BRS_LIST_DEFN(BRS_GUI_MenuList, BRS_GUI_Menu)

BRS_LIST_DEFN(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

BRS_LIST_DEFN(BRS_GUI_WidgetList, BRS_GUI_Widget)

BRS_GUI_Widget *BRS_GUI_createLabel(BRS_Point *position, const BRS_Color *color, const char *text, BRS_Font *font) {
    BRS_GUI_Label *label = malloc(sizeof(BRS_GUI_Label));
    label->text = text;
    label->position = position;
    label->font = font;
    label->color = color;

    BRS_GUI_Widget_Object *object = malloc(sizeof(BRS_GUI_Widget_Object));
    object->label = label;

    BRS_GUI_Widget *widget = malloc(sizeof(BRS_GUI_Widget));
    widget->type = BRS_GUI_WIDGET_LABEL;
    widget->object = object;
    return widget;
}

BRS_GUI_Widget *
BRS_GUI_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color, BRS_Font *font) {
    BRS_GUI_MenuBar *menubar = malloc(sizeof(BRS_GUI_MenuBar));
    menubar->color = color;
    menubar->dimension = dimension;
    menubar->menuList = BRS_GUI_MenuList_create();
    menubar->position = position;
    menubar->font = font;

    BRS_GUI_Widget_Object *object = malloc(sizeof(BRS_GUI_Widget_Object));
    object->menuBar = menubar;

    BRS_GUI_Widget *widget = malloc(sizeof(BRS_GUI_Widget));
    widget->type = BRS_GUI_WIDGET_MENUBAR;
    widget->object = object;
    return widget;
}

static void *BRS_GUI_renderLabel(BRS_VideoContext *context, BRS_GUI_Label *label) {
    BRS_setColor(context, label->color);
    BRS_drawString(context, label->text, label->font, label->position, label->color);
}

static void *BRS_GUI_renderMenu(BRS_VideoContext *context, BRS_GUI_Menu *menu, BRS_Point position) {
    BRS_drawString(context, menu->label, menu->font, &position, menu->color);
}

static void *BRS_GUI_renderMenuBar(BRS_VideoContext *context, BRS_GUI_MenuBar *menubar) {
    BRS_setColor(context, menubar->color);
    SDL_Rect r = {.x = menubar->position->x, .y = menubar->position->y, .w=menubar->dimension->width, .h=menubar->dimension->height};
    SDL_RenderFillRect(context->renderer, &r);

    BRS_GUI_Menu *menu = menubar->menuList->firstEntry->value;
    BRS_Point menuPos = {.x = menubar->position->x + 1, .y = menubar->position->y + 1};
    BRS_GUI_renderMenu(context, menu, menuPos);
}

void BRS_GUI_renderWidget(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_LABEL:
            BRS_GUI_renderLabel(context, widget->object->label);
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            BRS_GUI_renderMenuBar(context, widget->object->menuBar);
            break;
    }
}

static void destroyLabel(BRS_GUI_Label *label) {
    free(label);
}

static void destroyMenuBar(BRS_GUI_MenuBar *menuBar) {
    free(menuBar);
}

void BRS_GUI_destroyWidget(BRS_GUI_Widget *widget) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_LABEL:
            destroyLabel(widget->object->label);
            break;
        case BRS_GUI_WIDGET_MENUBAR:
            destroyMenuBar(widget->object->menuBar);
            break;
    }
    free(widget->object);
    free(widget);
}
