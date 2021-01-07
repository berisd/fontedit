//
// Created by bernd on 07.01.21.
//

#ifndef FONTEDIT_CHAR_TABLE_H
#define FONTEDIT_CHAR_TABLE_H

#include "../font.h"
#include "../video.h"

typedef struct _BRS_GUI_CharTable BRS_GUI_CharTable;

struct _BRS_GUI_CharTable {
    BRS_Font *font;
    BRS_Point *position;
    const BRS_Color *foreColor;
};

BRS_GUI_CharTable *
BRS_GUI_CharTable_create(BRS_Point *position, const BRS_Color *foreColor, BRS_Font *font);

void BRS_GUI_CharTable_destroy(BRS_GUI_CharTable *charTable);

void BRS_GUI_CharTable_render(BRS_VideoContext *context, BRS_GUI_CharTable *charTable);

#endif //FONTEDIT_CHAR_TABLE_H
