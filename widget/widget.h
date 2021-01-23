//
// Created by bernd on 02.01.21.
//

#ifndef BRS_GUI_WIDGET_H
#define BRS_GUI_WIDGET_H

#include "../video.h"
#include "../list.h"
#include "../theme.h"
#include "label.h"
#include "menu_item.h"
#include "menu.h"
#include "menu_bar.h"
#include "char_edit.h"
#include "char_table.h"
#include "messagebox.h"
#include "inputbox.h"
#include "window.h"

typedef enum _BRS_GUI_WidgetType BRS_GUI_WidgetType;

typedef union _BRS_GUI_Widget_Object BRS_GUI_Widget_Object;

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

union _BRS_GUI_Widget_Object {
    BRS_GUI_Window *window;
    BRS_GUI_MenuBar *menuBar;
    BRS_GUI_Label *label;
    BRS_GUI_CharEdit *charEdit;
    BRS_GUI_CharTable *charTable;
    BRS_GUI_MessageBox *messageBox;
    BRS_GUI_InputBox *inputBox;
};

struct _BRS_GUI_Widget {
    BRS_GUI_WidgetType type;
    BRS_GUI_Widget_Object *object;
    BRS_GUI_WidgetList *children;
};

void BRS_GUI_Widget_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget);

void BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

BRS_GUI_Widget *BRS_GUI_Widget_getByType(BRS_GUI_WidgetType type);

BRS_GUI_Widget *
BRS_GUI_Widget_createMenuBar(BRS_Point *position, BRS_Dimension *dimension, const BRS_GUI_Theme *theme);

BRS_GUI_Widget *
BRS_GUI_Widget_createLabel(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text);

BRS_GUI_Widget *
BRS_GUI_Widget_createMessageBox(BRS_Point *position, struct BRS_Dimension *dimension, const BRS_GUI_Theme *theme, const char *title, const char *text);

BRS_GUI_Widget *
BRS_GUI_Widget_createInputBox(BRS_Point *position, struct BRS_Dimension *dimension, const BRS_GUI_Theme *theme, const char *title, const char *textLabel);

BRS_GUI_Widget *
BRS_GUI_Widget_createCharEdit(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited);

BRS_GUI_Widget *
BRS_GUI_Widget_createCharTable(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited);

BRS_GUI_Widget *
BRS_GUI_Widget_createWindow();

void BRS_GUI_Widget_setClickHandler(BRS_GUI_Widget *widget, void *handler);

void BRS_GUI_Widget_addChild(BRS_GUI_Widget *widget, BRS_GUI_Widget *child);

#endif //BRS_GUI_WIDGET_H
