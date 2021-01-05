//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_LABEL_H
#define BRS_GUI_LABEL_H

#include "../video.h"

typedef struct _BRS_GUI_Label BRS_GUI_Label;

struct _BRS_GUI_Label {
    BRS_Point *position;
    const char *text;
    BRS_Font *font;
    const BRS_Color *color;
};

BRS_GUI_Label *BRS_GUI_Label_create(BRS_Point *position, const BRS_Color *color, const char *text, BRS_Font *font);

void *BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Label *label);

#endif //BRS_GUI_LABEL_H
