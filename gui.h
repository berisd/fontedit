//
// Created by bernd on 05.01.21.
//

#ifndef FONTEDIT_GUI_H
#define FONTEDIT_GUI_H

#include "widget/widget.h"
#include "theme.h"

typedef BRS_GUI_WidgetList BRS_GUI;

// External
extern BRS_GUI_WidgetList *getWidgetList();

extern void BRS_FontEdit_quitApplication();
// External End

BRS_GUI *BRS_GUI_createGUI(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight);

void BRS_GUI_destroyGUI(BRS_GUI *gui);

void BRS_GUI_initGUI(BRS_GUI_WidgetList *gui);

void BRS_GUI_renderGUI(BRS_VideoContext *videoContext, BRS_GUI *gui);

void BRS_GUI_processEvent(BRS_GUI *gui, SDL_Event *event);

#endif //FONTEDIT_GUI_H
