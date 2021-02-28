//
// Created by bernd on 07.01.21.
//

#ifndef FONTEDIT_CHAR_TABLE_H
#define FONTEDIT_CHAR_TABLE_H

#include "../font.h"
#include "widget.h"

typedef struct _BRS_GUI_CharTable BRS_GUI_CharTable;

typedef void (*BRS_GUI_CharTable_ChangedSelectedCharIndex)(BRS_GUI_CharTable *);

struct _BRS_GUI_CharTable {
    BRS_GUI_Widget widget;
    BRS_Font *fontEdited;
    int32_t highlightedCharIndex;
    int32_t selectedCharIndex;
    BRS_GUI_CharTable_ChangedSelectedCharIndex changedSelectedCharIndexHandler;
};

void BRS_GUI_CharTable_setSelectedCharIndex(BRS_GUI_CharTable *charTable, int32_t selectedCharIndex);

void BRS_GUI_CharTable_ctor(BRS_GUI_CharTable *charTable, BRS_GUI_Widget_Properties *widgetProps, BRS_Font *fontEdited);

void BRS_GUI_CharTable_dtor(BRS_GUI_CharTable *charTable);

BRS_GUI_CharTable *
BRS_GUI_CharTable_create(BRS_GUI_Widget_Properties *widgetProps, BRS_Font *fontEdited);

void BRS_GUI_CharTable_destroy(BRS_GUI_CharTable *charTable);

void
BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(BRS_GUI_CharTable *charTable,
                                                     BRS_GUI_CharTable_ChangedSelectedCharIndex handler);

#endif //FONTEDIT_CHAR_TABLE_H
