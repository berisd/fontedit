//
// Created by bernd on 07.01.21.
//

#ifndef FONTEDIT_CHAR_EDIT_H
#define FONTEDIT_CHAR_EDIT_H

#include "../font.h"
#include "../render.h"
#include "../theme.h"
#include "grid.h"
#include "widget.h"

typedef struct _BRS_GUI_CharEdit BRS_GUI_CharEdit;

struct _BRS_GUI_CharEdit {
    BRS_Font *fontEdited;
    int32_t selectedChar;
    uint8_t _buttonPressed;
};

BRS_GUI_CharEdit *
BRS_GUI_CharEdit_create(BRS_Font *fontEdited);

void BRS_GUI_CharEdit_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_CharEdit_destroy(BRS_GUI_CharEdit *charEdit);

bool BRS_GUI_CharEdit_processEvent(BRS_GUI_Widget *widget, SDL_Event *event);

#endif //FONTEDIT_CHAR_EDIT_H
