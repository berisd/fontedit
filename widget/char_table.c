//
// Created by bernd on 07.01.21.
//

#include "char_table.h"

static const uint16_t CHARS_PER_ROW = 32;
static const uint16_t CHAR_LINES = 8;
static const int32_t PIXELS_PADDING = 2;
static const int32_t PIXELS_BORDER = 1;
static const int32_t NO_CHAR = -1;

static int32_t calcTableWith(BRS_GUI_Widget *widget) {
    BRS_GUI_CharTable *charTable = widget->object;
    return CHARS_PER_ROW * (charTable->fontEdited->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2);
}

static int32_t calcTableHeight(BRS_GUI_Widget *widget) {
    BRS_GUI_CharTable *charTable = widget->object;
    return CHAR_LINES * (charTable->fontEdited->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2);
}

static int32_t calcCellWidth(const BRS_GUI_CharTable *charTable) {
    return charTable->fontEdited->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
}

static int32_t calcCellHeight(const BRS_GUI_CharTable *charTable) {
    return charTable->fontEdited->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
}

static void calcGridPosition(GridPosition *gridPosition, BRS_GUI_Widget *widget, BRS_Point *mousePoint) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_getFromWidget(widget);

    int32_t cellWidth = calcCellWidth(charTable);
    int32_t cellHeight = calcCellHeight(charTable);

    int32_t relX = mousePoint->x - widget->position->x;
    int32_t relY = mousePoint->y - widget->position->y;

    gridPosition->columnIndex = relX / cellWidth;
    gridPosition->rowIndex = relY / cellHeight;
}

static uint8_t getCharByGridPosition(GridPosition *gridPosition) {
    uint8_t ch = gridPosition->rowIndex * CHARS_PER_ROW + gridPosition->columnIndex;
    return ch;
}

static void processMouseMove(BRS_GUI_Widget *widget, BRS_Point *mousePoint) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_getFromWidget(widget);
    GridPosition gridPosition;
    calcGridPosition(&gridPosition, widget, mousePoint);
    uint8_t ch = getCharByGridPosition(&gridPosition);
    charTable->highlightedCharIndex = ch;
}

static void processMouseButtonDown(BRS_GUI_Widget *widget, BRS_Point *mousePoint) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_getFromWidget(widget);
    GridPosition gridPosition;
    calcGridPosition(&gridPosition, widget, mousePoint);
    uint8_t ch = getCharByGridPosition(&gridPosition);
    BRS_GUI_CharTable_setSelectedCharIndex(charTable, ch);

    if (charTable->clickHandler) {
        charTable->clickHandler(charTable);
    }
}

BRS_GUI_CharTable *
BRS_GUI_CharTable_create(BRS_Font *fontEdited) {
    BRS_GUI_CharTable *charTable = malloc(sizeof(BRS_GUI_CharTable));
    charTable->fontEdited = fontEdited;
    charTable->highlightedCharIndex = NO_CHAR;
    charTable->selectedCharIndex = NO_CHAR;
    charTable->clickHandler = NULL;
    charTable->changedSelectedCharIndexHandler = NULL;
    return charTable;
}

void BRS_GUI_CharTable_destroy(BRS_GUI_CharTable *charTable) {
    free(charTable);
}

static void drawTableBorder(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_setColor(context, widget->theme->borderColor);
    BRS_Rect rect = {
            .x = widget->position->x,
            .y = widget->position->y,
            .width = calcTableWith(widget),
            .height = calcTableHeight(widget)
    };
    BRS_drawlRect(context, &rect);
}

static void drawVerticalLines(const BRS_VideoContext *context, const BRS_GUI_Widget *widget) {
    BRS_GUI_CharTable *charTable = widget->object;
    BRS_setColor(context, widget->theme->borderColor);

    uint32_t i;
    BRS_Point p1 = {.x = widget->position->x, .y = widget->position->y};
    BRS_Point p2 = {.x = widget->position->x, .y = widget->position->y +
                                                   CHAR_LINES *
                                                   (charTable->fontEdited->height_bits + PIXELS_BORDER * 2 +
                                                    PIXELS_PADDING * 2)};
    BRS_Line line = {.p1 = &p1, .p2 = &p2};
    int32_t x_diff = charTable->fontEdited->width_bits + +PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
    for (i = 0; i < CHARS_PER_ROW; i++) {
        BRS_drawline(context, &line);
        line.p1->x += x_diff;
        line.p2->x += x_diff;
    }
}

static void drawHorizontalLines(const BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_getFromWidget(widget);
    BRS_setColor(context, widget->theme->borderColor);

    uint32_t i;
    BRS_Point p1 = {.x = widget->position->x, .y = widget->position->y};
    BRS_Point p2 = {.x = widget->position->x +
                         CHARS_PER_ROW * calcCellWidth(charTable), .y = widget->position->y};
    BRS_Line line = {.p1 = &p1, .p2 = &p2};
    int32_t y_diff = calcCellHeight(charTable);
    for (i = 0; i < CHAR_LINES; i++) {
        BRS_drawline(context, &line);
        line.p1->y += y_diff;
        line.p2->y += y_diff;
    }
}

static void drawChars(const BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_getFromWidget(widget);
    BRS_Point pos = {.x = widget->position->x + PIXELS_BORDER + PIXELS_PADDING, .y = widget->position->y +
                                                                                     PIXELS_BORDER + PIXELS_PADDING};
    BRS_Rect charRect = {.x = 0, .y = 0,
            .width = charTable->fontEdited->width_bits + 2 * PIXELS_PADDING,
            .height = charTable->fontEdited->height_bits + 2 * PIXELS_PADDING
    };
    int32_t x_diff = calcCellWidth(charTable);
    int32_t y_diff = calcCellHeight(charTable);

    uint16_t rowIndex;
    uint16_t colIndex;

    int32_t charIndex = 0;
    for (rowIndex = 0; rowIndex < CHAR_LINES; rowIndex++) {
        pos.x = widget->position->x + PIXELS_BORDER + PIXELS_PADDING;
        for (colIndex = 0; colIndex < CHARS_PER_ROW; colIndex++) {
            if (charTable->highlightedCharIndex == charIndex || charTable->selectedCharIndex == charIndex) {
                bool isSelected = charTable->selectedCharIndex == charIndex;
                const BRS_Color *charColor = isSelected ? widget->theme->charTableSelectedColor
                                                        : widget->theme->charTableHighlightedColor;
                BRS_setColor(context, charColor);
                charRect.x = pos.x - PIXELS_PADDING;
                charRect.y = pos.y - PIXELS_PADDING;
                BRS_drawlFillRect(context, &charRect);
            }

            uint8_t ch = charIndex;
            BRS_setColor(context, widget->theme->charTableCharColor);
            BRS_drawString(context, &ch, 1, charTable->fontEdited, &pos);
            pos.x += x_diff;
            charIndex++;
        }
        pos.y += y_diff;
    }
}

void BRS_GUI_CharTable_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    drawTableBorder(context, widget);
    drawVerticalLines(context, widget);
    drawHorizontalLines(context, widget);
    drawChars(context, widget);
}

bool BRS_GUI_CharTable_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    BRS_GUI_CharTable *charTable = widget->object;
    BRS_Rect widgetRect = {.x = widget->position->x, .y = widget->position->y, .width = calcTableWith(
            widget), .height = calcTableHeight(widget)};

    switch (event->type) {
        case SDL_MOUSEMOTION: {
            BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
            if (BRS_PointInRect(&mousePoint, &widgetRect)) {
                processMouseMove(widget, &mousePoint);
            } else {
                charTable->highlightedCharIndex = NO_CHAR;
            }
        }
            break;
        case SDL_MOUSEBUTTONUP: {
            BRS_Point mousePoint = {.x = event->button.x, .y = event->button.y};
            if (BRS_PointInRect(&mousePoint, &widgetRect)) {
                processMouseButtonDown(widget, &mousePoint);
            }
        }
            break;
    }
    return false;
}

BRS_GUI_CharTable *BRS_GUI_CharTable_getFromWidget(BRS_GUI_Widget *widget) {
    return widget->object;
}

void
BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(BRS_GUI_CharTable *charTable,
                                                     BRS_GUI_CharTable_ChangedSelectedCharIndex handler) {
    charTable->changedSelectedCharIndexHandler = handler;
}

void BRS_GUI_CharTable_setSelectedCharIndex(BRS_GUI_CharTable *charTable, int32_t selectedCharIndex) {
    if (charTable->selectedCharIndex == selectedCharIndex) {
        return;
    }

    charTable->selectedCharIndex = selectedCharIndex;
    if (charTable->changedSelectedCharIndexHandler != NULL) {
        charTable->changedSelectedCharIndexHandler(charTable);
    }
}