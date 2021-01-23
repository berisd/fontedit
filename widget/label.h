//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_LABEL_H
#define BRS_GUI_LABEL_H

#include "../render.h"
#include "../theme.h"

typedef struct _BRS_GUI_Label BRS_GUI_Label;

struct _BRS_GUI_Label {
    BRS_Point *position;
    BRS_GUI_Theme *theme;
    const char *text;
};

BRS_GUI_Label *BRS_GUI_Label_create(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text);

void BRS_GUI_Label_destroy(BRS_GUI_Label *label);

void BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Label *label);

#endif //BRS_GUI_LABEL_H
