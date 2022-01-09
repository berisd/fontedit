//
// Created by bernd on 05.01.21.
//

#include "label.h"

static void BRS_GUI_Label_render(const BRS_GUI_Widget *widget, const BRS_VideoContext *context) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_Label *label = (BRS_GUI_Label *) widget;
    BRS_setColor(context, widgetProps->theme->labelForeColor);
    BRS_drawString(context, label->text, strlen(label->text), widgetProps->theme->font, widgetProps->position);
}

void BRS_GUI_Label_ctor(BRS_GUI_Label *label, BRS_GUI_Widget_Properties *widgetProps, const char *text) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) label, widgetProps);
    label->text = malloc(255);
    strcpy(label->text, text);
    widgetProps->renderHandler = BRS_GUI_Label_render;
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_Label_destroy;
}

void BRS_GUI_Label_dtor(BRS_GUI_Label *label) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) label);
    free(label->text);
}

BRS_GUI_Label *BRS_GUI_Label_create(BRS_GUI_Widget_Properties *widgetProps, const char *text) {
    BRS_GUI_Label *label = malloc(sizeof(BRS_GUI_Label));
    BRS_GUI_Label_ctor(label, widgetProps, text);
    return label;
}

void BRS_GUI_Label_destroy(BRS_GUI_Label *label) {
    BRS_GUI_Label_dtor(label);
    free(label);
}

void BRS_GUI_Label_setText(BRS_GUI_Label *label, const char *text) {
    strcpy(label->text, text);
}
