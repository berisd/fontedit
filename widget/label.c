//
// Created by bernd on 05.01.21.
//

#include "label.h"

BRS_GUI_Label *BRS_GUI_Label_create(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text) {
    BRS_GUI_Label *label = malloc(sizeof(BRS_GUI_Label));
    label->text = malloc(255);
    memcpy(label->text, text, strlen(text));
    label->position = BRS_copyPoint(position);
    label->theme = (BRS_GUI_Theme *) theme;
    return label;
}

void BRS_GUI_Label_destroy(BRS_GUI_Label *label) {
    free(label->position);
    free(label->text);
    free(label);
}

void BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Label *label) {
    BRS_setColor(context, label->theme->labelForeColor);
    BRS_drawString(context, label->text, strlen(label->text), label->theme->font, label->position);
}

void BRS_GUI_Label_setText(BRS_GUI_Label* label, const char *text) {
    memset(label->text, 0, 255);
    memcpy(label->text, text, strlen(text));
}
