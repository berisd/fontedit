//
// Created by bernd on 05.01.21.
//

#include "label.h"

BRS_GUI_Label *BRS_GUI_Label_create(const char *text) {
    BRS_GUI_Label *label = malloc(sizeof(BRS_GUI_Label));
    label->text = malloc(255);
    memcpy(label->text, text, strlen(text));
    return label;
}

void BRS_GUI_Label_destroy(BRS_GUI_Label *label) {
    free(label->text);
    free(label);
}

void BRS_GUI_Label_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_Label *label = widget->object;
    BRS_setColor(context, widgetProps->theme->labelForeColor);
    BRS_drawString(context, label->text, strlen(label->text), widgetProps->theme->font, widgetProps->position);
}

void BRS_GUI_Label_setText(BRS_GUI_Widget *widget, const char *text) {
    BRS_GUI_Label *label = widget->object;
    memset(label->text, 0, 255);
    memcpy(label->text, text, strlen(text));
}

BRS_GUI_Label *BRS_GUI_Label_getFromWidget(BRS_GUI_Widget *widget) {
    return widget->object;
}
