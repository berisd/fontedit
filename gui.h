//
// Created by bernd on 05.01.21.
//

#ifndef FONTEDIT_GUI_H
#define FONTEDIT_GUI_H

#include "widget/widget.h"
#include "theme.h"

BRS_GUI_WidgetList *createWidgets(BRS_Font *font, uint32_t screenWidth, uint32_t screenHeight);

void destroyWidgets(BRS_GUI_WidgetList *widgetList);

void quitApplication();

BRS_GUI_WidgetList *getWidgetList();

#endif //FONTEDIT_GUI_H
