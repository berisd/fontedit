//
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
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
    widget->onClickHandler = malloc(sizeof(BRS_GUI_Widget_ClickHandler));
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

void BRS_GUI_processEvent(BRS_GUI_Widget *widget, SDL_Event event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int32_t x = event.button.x;
                int32_t y = event.button.y;
                switch (widget->type) {
                    case BRS_GUI_WIDGET_MENUBAR: {
                        BRS_GUI_MenuBar *menuBar = widget->object->menuBar;
                        BRS_GUI_MenuBar_ClickHandler clickHandler = widget->onClickHandler->menuBarClickHandler;
                        BRS_Point *position = menuBar->position;
                        BRS_Dimension *dimension = menuBar->dimension;
                        bool intersect = (x >= position->x && x <= position->x + dimension->width &&
                                          y >= position->y && y <= position->x + dimension->height);
                        if (intersect) {
                            clickHandler(menuBar);
                        }
                    }
                        break;
                }
            }
            break;
    }

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
    free(widget->onClickHandler);
    free(widget);
}

void BRS_GUI_setMenuBarClickHandler(BRS_GUI_Widget *widget, BRS_GUI_MenuBar_ClickHandler handler) {
    switch (widget->type) {
        case BRS_GUI_WIDGET_MENUBAR:
            widget->onClickHandler->menuBarClickHandler = handler;
            break;
    }
}
