//
// Created by bernd on 05.01.21.
//

#ifndef FONTEDIT_GUI_H
#define FONTEDIT_GUI_H

#include "widget/widget.h"

#define BRS_GUI_WIDGET_DEPTH_MAX 10

typedef struct _BRS_GUI {
    BRS_GUI_WidgetList *widgetList;
} BRS_GUI;

// External
extern void BRS_FontEdit_createFont();

extern bool BRS_FontEdit_loadFont(const char *filename);

extern bool BRS_FontEdit_saveFont(const char *filename);

extern void BRS_FontEdit_quitApplication();
// External End

BRS_GUI *BRS_GUI_create(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight);

void BRS_GUI_destroy(BRS_GUI *gui);

void BRS_GUI_init(BRS_GUI *gui);

void BRS_GUI_calculate(BRS_GUI *gui);

void BRS_GUI_render(BRS_GUI *gui, BRS_VideoContext *videoContext);

bool BRS_GUI_processEvent(BRS_GUI *gui, SDL_Event *event);

#endif //FONTEDIT_GUI_H
