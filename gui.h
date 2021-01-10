//
// Created by bernd on 05.01.21.
//

#ifndef FONTEDIT_GUI_H
#define FONTEDIT_GUI_H

#include "widget/widget.h"
#include "theme.h"

// External
extern BRS_GUI_WidgetList *getWidgetList();
// External End

BRS_GUI_WidgetList *createWidgets(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight);

void destroyWidgets(BRS_GUI_WidgetList *widgetList);

void quitApplication();

#endif //FONTEDIT_GUI_H
