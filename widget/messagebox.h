//
// Created by bernd on 10.01.21.
//

#ifndef FONTEDIT_MESSAGEBOX_H
#define FONTEDIT_MESSAGEBOX_H

#include "../render.h"
#include "../theme.h"

typedef struct _BRS_GUI_MessageBox BRS_GUI_MessageBox;

struct _BRS_GUI_MessageBox {
    BRS_Point *position;
    BRS_Size *size;
    BRS_GUI_Theme *theme;
    const char *title;
    const char *text;
};

BRS_GUI_MessageBox *
BRS_GUI_MessageBox_create(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme, const char *title,
                          const char *text);

void BRS_GUI_MessageBox_destroy(BRS_GUI_MessageBox *messageBox);

void BRS_GUI_MessageBox_render(BRS_VideoContext *context, BRS_GUI_MessageBox *messageBox);

bool BRS_GUI_MessageBox_processEvent(BRS_GUI_MessageBox *messageBox, SDL_Event *event);

#endif //FONTEDIT_MESSAGEBOX_H
