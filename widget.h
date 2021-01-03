//
// Created by bernd on 02.01.21.
//

#ifndef BRS_GUI_WIDGET_H
#define BRS_GUI_WIDGET_H

#include "video.h"
#include "list.h"

struct _BRS_GUI_Label {
    BRS_Point *position;
    const char *text;
    BRS_Font *font;
    const BRS_Color *color;
};
typedef struct _BRS_GUI_Label BRS_GUI_Label;

struct _BRS_GUI_MenuItem {
    char *label;
};
typedef struct _BRS_GUI_MenuItem BRS_GUI_MenuItem;

BRS_LIST_DECL(BRS_GUI_MenuItemList, BRS_GUI_MenuItem)

struct _BRS_GUI_Menu {
    char *label;
    const BRS_Color *color;
    BRS_Font *font;
    BRS_GUI_MenuItemList *itemList;
};
typedef struct _BRS_GUI_Menu BRS_GUI_Menu;

BRS_LIST_DECL(BRS_GUI_MenuList, BRS_GUI_Menu)

struct _BRS_GUI_MenuBar {
    BRS_Font *font;
    BRS_Point *position;
    BRS_Dimension *dimension;
    const BRS_Color *color;
    BRS_GUI_MenuList *menuList;
};
typedef struct _BRS_GUI_MenuBar BRS_GUI_MenuBar;

enum _BRS_GUI_WidgetType {
    BRS_GUI_WIDGET_LABEL,
    BRS_GUI_WIDGET_MENUBAR
};
typedef enum _BRS_GUI_WidgetType BRS_GUI_WidgetType;

union _BRS_GUI_Widget_Object {
    BRS_GUI_MenuBar *menuBar;
    BRS_GUI_Label *label;
};
typedef union _BRS_GUI_Widget_Object BRS_GUI_Widget_Object;

struct _BRS_GUI_Widget {
    BRS_GUI_WidgetType type;
    BRS_GUI_Widget_Object *object;
};
typedef struct _BRS_GUI_Widget BRS_GUI_Widget;

BRS_LIST_DECL(BRS_GUI_WidgetList, BRS_GUI_Widget)

void BRS_GUI_renderWidget(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_destroyWidget(BRS_GUI_Widget *widget);

BRS_GUI_Widget *BRS_GUI_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_Color *color, BRS_Font *font);

BRS_GUI_Widget *BRS_GUI_createLabel(BRS_Point *position, const BRS_Color *color, const char *text, BRS_Font *font);

BRS_GUI_Widget *BRS_GUI_createMenu();

BRS_GUI_Widget *BRS_GUI_createMenuItem();

#endif //BRS_GUI_WIDGET_H
