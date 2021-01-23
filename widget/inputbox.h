//
// Created by bernd on 10.01.21.
//

#ifndef FONTEDIT_INPUTBOX_H
#define FONTEDIT_INPUTBOX_H

#include "../render.h"
#include "../theme.h"

typedef struct _BRS_GUI_InputBox BRS_GUI_InputBox;

struct _BRS_GUI_InputBox {
    BRS_Point *position;
    BRS_Size *size;
    BRS_GUI_Theme *theme;
    const char *title;
    const char *textLabel;
    uint8_t *text;
    bool visible;
};

BRS_GUI_InputBox *
BRS_GUI_InputBox_create(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme, const char *title,
                        const char *textLabel);

void BRS_GUI_InputBox_destroy(BRS_GUI_InputBox *inputBox);

void BRS_GUI_InputBox_render(BRS_VideoContext *context, BRS_GUI_InputBox *inputBox);

void BRS_GUI_InputBox_processEvent(BRS_GUI_InputBox *inputBox, SDL_Event *event);

void BRS_GUI_InputBox_clearText(BRS_GUI_InputBox *inputBox);

#endif //FONTEDIT_INPUTBOX_H
