//
// Created by bernd on 09.01.21.
//

#include "theme.h"

const BRS_Color COLOR_RED = {.r = 255, .g = 0, .b = 0, .a = 255};
const BRS_Color COLOR_BLUE = {.r = 0, .g = 0, .b = 255, .a = 255};
const BRS_Color COLOR_YELLOW = {.r = 255, .g = 255, .b = 0, .a = 255};
const BRS_Color COLOR_WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
const BRS_Color COLOR_BLACK = {.r = 0, .g = 0, .b = 0, .a = 0};
const BRS_Color COLOR_DARK_GRAY = {.r = 135, .g = 134, .b = 131, .a = 255};
const BRS_Color COLOR_LIGHT_GRAY = {.r = 180, .g = 179, .b = 177, .a = 255};

BRS_GUI_Theme *BRS_GUI_Theme_create() {
    BRS_GUI_Theme *theme = malloc(sizeof(BRS_GUI_Theme));
    theme->font = createDefaultFont();
    theme->screenColor = &COLOR_BLACK;
    theme->menuBarColor = &COLOR_BLUE;
    theme->menuBackColor = &COLOR_BLUE;
    theme->menuForeColor = &COLOR_WHITE;
    theme->menuSelectedForeColor = &COLOR_DARK_GRAY;
    theme->menuItemBackColor = &COLOR_BLUE;
    theme->menuItemForeColor = &COLOR_WHITE;
    theme->menuItemHighlightedColor = &COLOR_LIGHT_GRAY;
    theme->labelForeColor = &COLOR_LIGHT_GRAY;
    theme->charTableBorderColor = &COLOR_RED;
    theme->charTableCharColor = &COLOR_YELLOW;
    theme->charTableHighlightedColor = &COLOR_LIGHT_GRAY;
    theme->charTableSelectedColor = &COLOR_DARK_GRAY;
    theme->charEditForeColor = &COLOR_RED;
    theme->charEditDotColor = &COLOR_YELLOW;
    return theme;
}

void BRS_GUI_Theme_destroy(BRS_GUI_Theme *theme) {
    free(theme->font);
    free(theme);
}