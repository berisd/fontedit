//
// Created by bernd on 07.01.21.
//

#ifndef FONTEDIT_CHAR_EDIT_H
#define FONTEDIT_CHAR_EDIT_H

#include "../font.h"
#include "widget.h"

typedef struct _BRS_GUI_CharEdit BRS_GUI_CharEdit;

struct _BRS_GUI_CharEdit {
    BRS_GUI_Widget widget;
    BRS_Font *fontEdited;
    int32_t selectedChar;
    uint8_t _buttonPressed;
};

BRS_GUI_CharEdit *
BRS_GUI_CharEdit_create(BRS_GUI_Widget_Properties *widgetProps, BRS_Font *fontEdited);

void BRS_GUI_CharEdit_destroy(BRS_GUI_CharEdit *charEdit);

void BRS_GUI_CharEdit_ctor(BRS_GUI_CharEdit *charEdit, BRS_GUI_Widget_Properties *widgetProps, BRS_Font *fontEdited);

void BRS_GUI_CharEdit_dtor(BRS_GUI_CharEdit *charEdit);

#endif //FONTEDIT_CHAR_EDIT_H
