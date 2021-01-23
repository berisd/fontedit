//
// Created by bernd on 10.01.21.
//

#include "messagebox.h"

BRS_GUI_MessageBox *
BRS_GUI_MessageBox_create(BRS_Point *position, BRS_Size *size, const BRS_GUI_Theme *theme, const char *title,
                          const char *text) {
    BRS_GUI_MessageBox *messageBox = malloc(sizeof(BRS_GUI_MessageBox));
    messageBox->theme = (BRS_GUI_Theme *) theme;
    messageBox->position = BRS_copyPoint(position);
    messageBox->size = BRS_copySize(size);
    messageBox->title = title;
    messageBox->text = text;
    return messageBox;
}

void BRS_GUI_MessageBox_destroy(BRS_GUI_MessageBox *messageBox) {
    free(messageBox->position);
    free(messageBox->size);
    free(messageBox);
}

void BRS_GUI_MessageBox_render(BRS_VideoContext *context, BRS_GUI_MessageBox *messageBox) {

}

void BRS_GUI_MessageBox_processEvent(BRS_GUI_MessageBox *messageBox, SDL_Event *event) {

}