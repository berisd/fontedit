//
// Created by bernd on 10.01.21.
//

#ifndef FONTEDIT_INPUTBOX_H
#define FONTEDIT_INPUTBOX_H

#include "widget.h"

typedef struct _BRS_GUI_InputBox BRS_GUI_InputBox;

typedef void (*BRS_GUI_InputBox_ConfirmHandler)(BRS_GUI_InputBox *);

typedef void (*BRS_GUI_InputBox_CancelHandler)(BRS_GUI_InputBox *);

struct _BRS_GUI_InputBox {
    BRS_GUI_Widget widget;
    char *title;
    char *textLabel;
    char *text;
    BRS_GUI_InputBox_ConfirmHandler confirmHandler;
    BRS_GUI_InputBox_CancelHandler cancelHandler;
};

BRS_GUI_InputBox *
BRS_GUI_InputBox_create(BRS_GUI_Widget_Properties *widgetProps, const char *title, const char *textLabel);

void BRS_GUI_InputBox_destroy(BRS_GUI_InputBox *inputBox);

void BRS_GUI_InputBox_clearText(BRS_GUI_InputBox *inputBox);

void BRS_GUI_InputBox_setConfirmHandler(BRS_GUI_InputBox *inputBox, BRS_GUI_InputBox_ConfirmHandler handler);

void BRS_GUI_InputBox_setCancelHandler(BRS_GUI_InputBox *inputBox, BRS_GUI_InputBox_CancelHandler handler);

void BRS_GUI_InputBox_setTitle(BRS_GUI_InputBox *inputBox, const char *title);

void BRS_GUI_InputBox_setTextLabel(BRS_GUI_InputBox *inputBox, const char *textLabel);

#endif //FONTEDIT_INPUTBOX_H
