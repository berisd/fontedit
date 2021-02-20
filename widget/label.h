//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_LABEL_H
#define BRS_GUI_LABEL_H

#include "../render.h"
#include "../theme.h"
#include "widget.h"

typedef struct _BRS_GUI_Label BRS_GUI_Label;

struct _BRS_GUI_Label {
    char *text;
};

BRS_GUI_Label *BRS_GUI_Label_create(const char *text);

void BRS_GUI_Label_destroy(BRS_GUI_Label *label);

void BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Widget *widget);

void BRS_GUI_Label_setText(BRS_GUI_Widget *widget, const char *text);

BRS_GUI_Label *BRS_GUI_Label_getFromWidget(BRS_GUI_Widget *widget);

#endif //BRS_GUI_LABEL_H
