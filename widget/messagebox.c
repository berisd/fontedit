//
// Created by bernd on 10.01.21.
//

#include "messagebox.h"

BRS_GUI_MessageBox *
BRS_GUI_MessageBox_create(const char *title, const char *text) {
    BRS_GUI_MessageBox *messageBox = malloc(sizeof(BRS_GUI_MessageBox));
    messageBox->title = title;
    messageBox->text = text;
    return messageBox;
}

void BRS_GUI_MessageBox_destroy(BRS_GUI_MessageBox *messageBox) {
    free(messageBox);
}

void BRS_GUI_MessageBox_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {

}

bool BRS_GUI_MessageBox_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    return false;
}