//
// Created by bernd on 10.01.21.
//

#include <string.h>
#include "messagebox.h"

void BRS_GUI_MessageBox_ctor(BRS_GUI_MessageBox *messageBox, BRS_GUI_Widget_Properties *widgetProps, const char *title,
                             const char *text) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) messageBox, widgetProps);
    messageBox->title = strdup(title);
    messageBox->text = strdup(text);
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_MessageBox_destroy;
}

void BRS_GUI_MessageBox_dtor(BRS_GUI_MessageBox *messageBox) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) messageBox);
    free(messageBox->title);
    free(messageBox->text);
}

BRS_GUI_MessageBox *
BRS_GUI_MessageBox_create(BRS_GUI_Widget_Properties *widgetProps, const char *title, const char *text) {
    BRS_GUI_MessageBox *messageBox = malloc(sizeof(BRS_GUI_MessageBox));
    BRS_GUI_MessageBox_ctor(messageBox, widgetProps, title, text);
    return messageBox;
}

void BRS_GUI_MessageBox_destroy(BRS_GUI_MessageBox *messageBox) {
    BRS_GUI_MessageBox_dtor(messageBox);
    free(messageBox);
}

