//
// Created by bernd on 05.01.21.
//

#include "label.h"

BRS_GUI_Label *BRS_GUI_Label_create(BRS_Point *position, const BRS_Color *color, const char *text, BRS_Font *font) {
    BRS_GUI_Label *label = malloc(sizeof(BRS_GUI_Label));
    label->text = text;
    label->position = position;
    label->font = font;
    label->color = color;
    return label;
}

void *BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Label *label) {
    BRS_setColor(context, label->color);
    BRS_drawString(context, label->text, label->font, label->position, label->color);
}

