//
// Created by bernd on 07.01.21.
//

#ifndef FONTEDIT_CHAR_EDIT_H
#define FONTEDIT_CHAR_EDIT_H

#include "../font.h"
#include "../video.h"
#include "../theme.h"
#include "grid.h"

typedef struct _BRS_GUI_CharEdit BRS_GUI_CharEdit;

struct _BRS_GUI_CharEdit {
    BRS_Font *fontEdited;
    BRS_Point *position;
    int32_t selectedChar;
    BRS_GUI_Theme *theme;
    uint8_t _buttonPressed;
};

BRS_GUI_CharEdit *
BRS_GUI_CharEdit_create(BRS_Point *position, const BRS_GUI_Theme *theme, BRS_Font *fontEdited);

void BRS_GUI_CharEdit_render(BRS_VideoContext *context, BRS_GUI_CharEdit *charEdit);

void BRS_GUI_CharEdit_destroy(BRS_GUI_CharEdit *charEdit);

void BRS_GUI_CharEdit_processEvent(BRS_GUI_CharEdit *charEdit, SDL_Event *event);

#endif //FONTEDIT_CHAR_EDIT_H
