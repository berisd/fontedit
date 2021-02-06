//
// Created by bernd on 10.01.21.
//

#ifndef FONTEDIT_INPUTBOX_H
#define FONTEDIT_INPUTBOX_H

#include "../render.h"
#include "../theme.h"
#include "widget.h"

typedef struct _BRS_GUI_InputBox BRS_GUI_InputBox;

typedef void (*BRS_GUI_InputBox_ConfirmHandler)(BRS_GUI_InputBox *);

typedef void (*BRS_GUI_InputBox_CancelHandler)(BRS_GUI_InputBox *);

struct _BRS_GUI_InputBox {
    const char *title;
    const char *textLabel;
    uint8_t *text;
    BRS_GUI_InputBox_ConfirmHandler confirmHandler;
    BRS_GUI_InputBox_CancelHandler cancelHandler;
};

BRS_GUI_InputBox *
BRS_GUI_InputBox_create(const char *title, const char *textLabel);

void BRS_GUI_InputBox_destroy(BRS_GUI_InputBox *inputBox);

void BRS_GUI_InputBox_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

bool BRS_GUI_InputBox_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

BRS_GUI_InputBox *BRS_GUI_InputBox_getFromWidget(BRS_GUI_Widget *widget);

void BRS_GUI_InputBox_clearText(BRS_GUI_InputBox *inputBox);

void BRS_GUI_InputBox_setConfirmHandler(BRS_GUI_InputBox *inputBox, BRS_GUI_InputBox_ConfirmHandler handler);

void BRS_GUI_InputBox_setCancelHandler(BRS_GUI_InputBox *inputBox, BRS_GUI_InputBox_CancelHandler handler);

#endif //FONTEDIT_INPUTBOX_H
