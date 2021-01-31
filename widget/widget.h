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

BRS_LIST_DECL(BRS_GUI_WidgetList, BRS_GUI_Widget)

enum _BRS_GUI_WidgetType {
    BRS_GUI_WIDGET_LABEL,
    BRS_GUI_WIDGET_MENUBAR,
    BRS_GUI_WIDGET_CHAREDIT,
    BRS_GUI_WIDGET_CHARTABLE,
    BRS_GUI_WIDGET_MESSAGEBOX,
    BRS_GUI_WIDGET_INPUTBOX,
    BRS_GUI_WIDGET_WINDOW
};

struct _BRS_GUI_Widget {
    BRS_GUI_WidgetType type;
    void *object;
    BRS_GUI_WidgetList *children;
    BRS_Point *position;
    BRS_Size *size;
    const BRS_GUI_Theme *theme;
};

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget);

bool BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

BRS_GUI_Widget *BRS_GUI_Widget_getByType(BRS_GUI_WidgetType type, BRS_GUI_WidgetList *widgetList);

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme);

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text);

BRS_GUI_Widget *
BRS_GUI_Widget_createMessageBox(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme, const char *title,
                                const char *text);

BRS_GUI_Widget *
BRS_GUI_Widget_createInputBox(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme, const char *title,
                              const char *textLabel);

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited);

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited);

BRS_GUI_Widget *
BRS_GUI_Widget_createWindow(BRS_Point *position, BRS_Size *size, BRS_GUI_Theme *theme);

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, void *handler);

#endif //BRS_GUI_WIDGET_H
