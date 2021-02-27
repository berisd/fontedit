//
// Created by bernd on 02.01.21.
//

#ifndef BRS_GUI_WIDGET_H
#define BRS_GUI_WIDGET_H

#include "../video.h"
#include "../render.h"
#include "../list.h"
#include "../theme.h"

typedef struct _BRS_GUI_Widget BRS_GUI_Widget;

typedef struct _BRS_GUI_Widget_Properties BRS_GUI_Widget_Properties;

typedef void (*BRS_GUI_Widget_ClickHandler)(BRS_GUI_Widget *);

typedef void (*BRS_GUI_Widget_RenderHandler)(const BRS_GUI_Widget *, const BRS_VideoContext *);

typedef bool (*BRS_GUI_Widget_ProcessEventHandler)(BRS_GUI_Widget *, SDL_Event *);

typedef void (*BRS_GUI_Widget_ExecuteActionHandler)(BRS_GUI_Widget *, const char *);

typedef void (*BRS_GUI_Widget_CalculateHandler)(BRS_GUI_Widget *);

typedef void (*BRS_GUI_Widget_DestroyHandler)(BRS_GUI_Widget *);

BRS_LIST_DECL(BRS_GUI_WidgetList, BRS_GUI_Widget)

struct _BRS_GUI_Widget_Properties {
    char *id;
    BRS_GUI_Widget *parent;
    BRS_GUI_WidgetList *children;
    BRS_Point *position;
    BRS_Size *size;
    int16_t zIndex;
    bool visible;
    BRS_GUI_Theme *theme;
    BRS_Padding *padding;
    BRS_GUI_Widget_ClickHandler clickHandler;
    BRS_GUI_Widget_ProcessEventHandler processEventHandler;
    BRS_GUI_Widget_ExecuteActionHandler executeActionHandler;
    BRS_GUI_Widget_RenderHandler renderHandler;
    BRS_GUI_Widget_CalculateHandler calculateHandler;
    BRS_GUI_Widget_DestroyHandler destroyHandler;
};

struct _BRS_GUI_Widget {
    BRS_GUI_Widget_Properties *properties;
};

void BRS_GUI_Widget_ctor(BRS_GUI_Widget *widget, BRS_GUI_Widget_Properties *widgetProps);

void BRS_GUI_Widget_dtor(BRS_GUI_Widget *widget);

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_create(const char *id);

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_copy(BRS_GUI_Widget_Properties *source);

void BRS_GUI_Widget_Properties_destroy(BRS_GUI_Widget_Properties *properties);

BRS_GUI_Widget *BRS_GUI_Widget_findRootWidget(BRS_GUI_Widget *widget);

BRS_GUI_Widget *BRS_GUI_Widget_findWidgetById(const char *id, BRS_GUI_Widget *rootWidget);

BRS_GUI_Widget *BRS_GUI_Widget_getByIdFromList(const char *id, BRS_GUI_WidgetList *widgetList);

void BRS_GUI_Widget_render(BRS_GUI_Widget *widget, BRS_VideoContext *context);

bool BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget);

void BRS_GUI_Widget_calculate(BRS_GUI_Widget *widget);

void BRS_GUI_Widget_setVisible(BRS_GUI_Widget *widget, bool visible);

#endif //BRS_GUI_WIDGET_H
