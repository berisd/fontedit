//
// Created by bernd on 02.01.21.
//

#ifndef BRS_GUI_WIDGET_H
#define BRS_GUI_WIDGET_H

#include "../video.h"
#include "../render.h"
#include "../list.h"
#include "../theme.h"

typedef enum _BRS_GUI_WidgetType BRS_GUI_WidgetType;

typedef struct _BRS_GUI_Widget BRS_GUI_Widget;

typedef struct _BRS_GUI_Widget_Properties BRS_GUI_Widget_Properties;

typedef void (*BRS_GUI_Widget_ClickHandler)(BRS_GUI_Widget *);

BRS_LIST_DECL(BRS_GUI_WidgetList, BRS_GUI_Widget)

enum _BRS_GUI_WidgetType {
    BRS_GUI_WIDGET_LABEL,
    BRS_GUI_WIDGET_MENU,
    BRS_GUI_WIDGET_MENU_ITEM,
    BRS_GUI_WIDGET_MENUBAR,
    BRS_GUI_WIDGET_CHAREDIT,
    BRS_GUI_WIDGET_CHARTABLE,
    BRS_GUI_WIDGET_MESSAGEBOX,
    BRS_GUI_WIDGET_INPUTBOX,
    BRS_GUI_WIDGET_WINDOW
};

struct _BRS_GUI_Widget_Properties {
    char *id;
    BRS_Point *position;
    BRS_Size *size;
    int16_t zIndex;
    bool visible;
    BRS_GUI_Theme *theme;
    BRS_Padding *padding;
};

struct _BRS_GUI_Widget {
    BRS_GUI_WidgetType type;
    void *object;
    BRS_GUI_Widget *parent;
    BRS_GUI_WidgetList *children;
    BRS_GUI_Widget_Properties *properties;
    BRS_GUI_Widget_ClickHandler clickHandler;
};

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget);

void BRS_GUI_Widget_calculate(BRS_GUI_Widget *widget, BRS_GUI_Widget *parentWidget, int32_t widgetIndex);

bool BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

BRS_GUI_Widget *BRS_GUI_Widget_getByType(BRS_GUI_WidgetType type, BRS_GUI_WidgetList *widgetList);

BRS_GUI_Widget *BRS_GUI_Widget_getByData(void *data, BRS_GUI_WidgetList *widgetList);

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_GUI_Widget_Properties *properties, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createMenu(BRS_GUI_Widget_Properties *properties, void *menu, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_GUI_Widget_Properties *properties, const char *text, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createMessageBox(BRS_GUI_Widget_Properties *properties, const char *title, const char *text, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createInputBox(BRS_GUI_Widget_Properties *properties, const char *title, const char *textLabel, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_GUI_Widget_Properties *properties, BRS_Font *fontEdited, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_GUI_Widget_Properties *properties, BRS_Font *fontEdited, BRS_GUI_Widget *parent);

BRS_GUI_Widget *
BRS_GUI_Widget_createWindow(BRS_GUI_Widget_Properties *properties);

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_create(const char *id);

void BRS_GUI_Widget_Properties_destroy(BRS_GUI_Widget_Properties *properties);

void BRS_GUI_Widget_executeAction(BRS_GUI_Widget *widget, const char *action);

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, BRS_GUI_Widget_ClickHandler handler);

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuItem(BRS_GUI_Widget_Properties *properties, void *menuItem, BRS_GUI_Widget *parentWidget);

BRS_GUI_Widget *BRS_GUI_Widget_findRootWidget(BRS_GUI_Widget *widget);

BRS_GUI_Widget *BRS_GUI_Widget_findWidgetById(const char *id, BRS_GUI_Widget *rootWidget);

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_copy(BRS_GUI_Widget_Properties* source);

#endif //BRS_GUI_WIDGET_H
