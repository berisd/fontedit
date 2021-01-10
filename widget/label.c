//
// Created by bernd on 05.01.21.
//

#include "label.h"

BRS_GUI_Label *BRS_GUI_Label_create(BRS_Point *position, const BRS_GUI_Theme *theme, const char *text) {
    BRS_GUI_Label *label = malloc(sizeof(BRS_GUI_Label));
    label->text = text;
    label->position = BRS_copyPoint(position);
    label->theme = (BRS_GUI_Theme *)theme;
    return label;
}

void BRS_GUI_Label_destroy(BRS_GUI_Label *label) {
    free(label->position);
    free(label);
}

void *BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Label *label) {
    BRS_drawString(context, label->text, strlen(label->text), label->theme->font, label->position,
                   label->theme->labelForeColor);
}

