//
// Created by bernd on 07.01.21.
//

#ifndef FONTEDIT_CHAR_TABLE_H
#define FONTEDIT_CHAR_TABLE_H

#include "../font.h"
#include "../video.h"
#include "grid.h"

typedef struct _BRS_GUI_CharTable BRS_GUI_CharTable;

typedef void (*BRS_GUI_CharTable_ClickHandler)(BRS_GUI_CharTable *);

typedef void (*BRS_GUI_CharTable_ChangedSelectedCharIndex)(BRS_GUI_CharTable *);

struct _BRS_GUI_CharTable {
    BRS_Font *fontEdited;
    BRS_Point *position;
    const BRS_Color *borderColor;
    const BRS_Color *charColor;
    const BRS_Color *highlightedColor;
    const BRS_Color *selectedColor;
    int32_t highlightedCharIndex;
    int32_t selectedCharIndex;
    BRS_GUI_CharTable_ClickHandler clickHandler;
    BRS_GUI_CharTable_ChangedSelectedCharIndex changedSelectedCharIndexHandler;
};

void BRS_GUI_CharTable_setSelectedCharIndex(BRS_GUI_CharTable *charTable, int32_t selectedCharIndex);

BRS_GUI_CharTable *
BRS_GUI_CharTable_create(BRS_Point *position, const BRS_Color *borderColor, const BRS_Color *charColor,
                         const BRS_Color *highlightedColor, const BRS_Color *selectedColor, BRS_Font *fontEdited);

void BRS_GUI_CharTable_destroy(BRS_GUI_CharTable *charTable);

void BRS_GUI_CharTable_render(BRS_VideoContext *context, BRS_GUI_CharTable *charTable);

void BRS_GUI_CharTable_processEvent(BRS_GUI_CharTable *charTable, SDL_Event *event);

void BRS_GUI_CharTable_setClickHandler(BRS_GUI_CharTable *charTable, BRS_GUI_CharTable_ClickHandler handler);

void
BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(BRS_GUI_CharTable *charTable,
                                                     BRS_GUI_CharTable_ChangedSelectedCharIndex handler);

#endif //FONTEDIT_CHAR_TABLE_H
