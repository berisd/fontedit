//
// Created by bernd on 05.01.21.
//

#ifndef BRS_GUI_LABEL_H
#define BRS_GUI_LABEL_H

#include "widget.h"

typedef struct _BRS_GUI_Label BRS_GUI_Label;

struct _BRS_GUI_Label {
    BRS_GUI_Widget widget;
    char *text;
};

void BRS_GUI_Label_ctor(BRS_GUI_Label *label, BRS_GUI_Widget_Properties *widgetProps, const char *text);

void BRS_GUI_Label_dtor(BRS_GUI_Label *label);

BRS_GUI_Label *BRS_GUI_Label_create(BRS_GUI_Widget_Properties *widgetProps, const char *text);

void BRS_GUI_Label_destroy(BRS_GUI_Label *label);

void BRS_GUI_Label_setText(BRS_GUI_Label *label, const char *text);

#endif //BRS_GUI_LABEL_H
