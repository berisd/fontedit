/**
 * Window displayed on GUI
 */

#ifndef FONTEDIT_WINDOW_H
#define FONTEDIT_WINDOW_H

#include <stdlib.h>

typedef struct _BRS_GUI_Window {
} BRS_GUI_Window;

BRS_GUI_Window *BRS_GUI_Window_create();

void BRS_GUI_Window_destroy(BRS_GUI_Window *window);


#endif //FONTEDIT_WINDOW_H
