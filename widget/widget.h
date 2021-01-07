//
// Created by bernd on 02.01.21.
//

#ifndef BRS_GUI_WIDGET_H
#define BRS_GUI_WIDGET_H

#include "../video.h"
#include "../list.h"
#include "label.h"
#include "menu_item.h"
#include "menu.h"
#include "menu_bar.h"

typedef enum _BRS_GUI_WidgetType BRS_GUI_WidgetType;

typedef union _BRS_GUI_Widget_Object BRS_GUI_Widget_Object;

typedef struct _BRS_GUI_Widget BRS_GUI_Widget;

BRS_LIST_DECL(BRS_GUI_WidgetList, BRS_GUI_Widget)

enum _BRS_GUI_WidgetType {
    BRS_GUI_WIDGET_LABEL,
    BRS_GUI_WIDGET_MENUBAR
};

union _BRS_GUI_Widget_Object {
    BRS_GUI_MenuBar *menuBar;
    BRS_GUI_Label *label;
};

struct _BRS_GUI_Widget {
    BRS_GUI_WidgetType type;
    BRS_GUI_Widget_Object *object;
};

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_destroyWidget(BRS_GUI_Widget *widget);

void BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

BRS_GUI_Widget *BRS_GUI_Widget_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *foreColor, BRS_Font *font);

BRS_GUI_Widget *BRS_GUI_Widget_createLabel(BRS_Point *position, const BRS_Color *color, const char *text, BRS_Font *font);

void BRS_GUI_setMenuBarClickHandler(BRS_GUI_Widget *widget, BRS_GUI_MenuBar_ClickHandler handler);

#endif //BRS_GUI_WIDGET_H
