//
// Created by bernd on 09.01.21.
//

#ifndef FONTEDIT_THEME_H
#define FONTEDIT_THEME_H

#include "video.h"
#include "color.h"
#include "util.h"

extern const BRS_Color COLOR_RED;
extern const BRS_Color COLOR_BLUE;
extern const BRS_Color COLOR_YELLOW;
extern const BRS_Color COLOR_WHITE;
extern const BRS_Color COLOR_BLACK;
extern const BRS_Color COLOR_DARK_GRAY;
extern const BRS_Color COLOR_LIGHT_GRAY;

typedef struct _BRS_GUI_Theme {
    BRS_Font *font;
    const BRS_Color *screenColor;
    const BRS_Color *menuBarColor;
    const BRS_Color *menuForeColor;
    const BRS_Color *menuBackColor;
    const BRS_Color *menuSelectedForeColor;
    const BRS_Color *menuItemForeColor;
    const BRS_Color *menuItemBackColor;
    const BRS_Color *menuItemHighlightedColor;
    const BRS_Color *labelForeColor;
    const BRS_Color *borderColor;
    const BRS_Color *charTableCharColor;
    const BRS_Color *charTableHighlightedColor;
    const BRS_Color *charTableSelectedColor;
    const BRS_Color *charEditForeColor;
    const BRS_Color *charEditDotColor;
    const BRS_Color *inputBoxTextForeColor;
    const BRS_Color *inputBoxTitleForeColor;
    const BRS_Color *inputBoxBackColor;
} BRS_GUI_Theme;

BRS_GUI_Theme *BRS_GUI_Theme_create();
void BRS_GUI_Theme_destroy(BRS_GUI_Theme *theme);

#endif //FONTEDIT_THEME_H
