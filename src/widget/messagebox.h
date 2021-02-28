//
// Created by bernd on 10.01.21.
//

#ifndef FONTEDIT_MESSAGEBOX_H
#define FONTEDIT_MESSAGEBOX_H

#include "widget.h"

typedef struct _BRS_GUI_MessageBox BRS_GUI_MessageBox;

struct _BRS_GUI_MessageBox {
    BRS_GUI_Widget widget;
    char *title;
    char *text;
};

void BRS_GUI_MessageBox_ctor(BRS_GUI_MessageBox *messageBox, BRS_GUI_Widget_Properties *widgetProps, const char *title,
                             const char *text);

void BRS_GUI_MessageBox_dtor(BRS_GUI_MessageBox *messageBox);

BRS_GUI_MessageBox *
BRS_GUI_MessageBox_create(BRS_GUI_Widget_Properties *widgetProps, const char *title, const char *text);

void BRS_GUI_MessageBox_destroy(BRS_GUI_MessageBox *messageBox);


#endif //FONTEDIT_MESSAGEBOX_H
