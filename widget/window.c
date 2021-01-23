//
// Created by bernd on 23.01.21.
//

#include "window.h"

BRS_GUI_Window *BRS_GUI_Window_create() {
    return malloc(sizeof(BRS_GUI_Window));
}

void BRS_GUI_Window_destroy(BRS_GUI_Window *window) {
    free(window);
}