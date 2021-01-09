//
// Created by bernd on 07.01.21.
//

#include "char_table.h"

static const uint16_t CHARS_PER_ROW = 32;
static const uint16_t CHAR_LINES = 8;
static const int32_t PIXELS_PADDING = 2;
static const int32_t PIXELS_BORDER = 1;
static const int32_t NO_CHAR = -1;

static int32_t calcTableWith(BRS_GUI_CharTable *charTable) {
    return CHARS_PER_ROW * (charTable->font->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2);
}

static int32_t calcTableHeight(BRS_GUI_CharTable *charTable) {
    return CHAR_LINES * (charTable->font->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2);
}

static int32_t calcCellWith(BRS_GUI_CharTable *charTable) {
    return charTable->font->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
}

static int32_t calcCellHeight(BRS_GUI_CharTable *charTable) {
    return charTable->font->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
}

static bool calcGridPosition(GridPosition *gridPosition, BRS_GUI_CharTable *charTable, int32_t x, int32_t y) {
    int32_t cellWidth = calcCellWith(charTable);
    int32_t cellHeight = calcCellHeight(charTable);

    BRS_Point point = {.x = x, .y = y};
    BRS_Rect cellRect = {.x = 0, .y = 0, .width = cellWidth, .height = cellHeight};
    for (uint16_t rowIndex = 0; rowIndex < CHAR_LINES; rowIndex++) {
        for (uint16_t colIndex = 0; colIndex < CHARS_PER_ROW; colIndex++) {
            cellRect.x = charTable->position->x + cellWidth * colIndex;
            cellRect.y = charTable->position->y + cellHeight * rowIndex;
            if (BRS_PointInRect(&point, &cellRect)) {
                gridPosition->columnIndex = colIndex;
                gridPosition->rowIndex = rowIndex;
                return true;
            }
        }
    }
    return false;
}

static uint8_t getCharByGridPosition(GridPosition *gridPosition) {
    uint8_t ch = gridPosition->rowIndex * CHARS_PER_ROW + gridPosition->columnIndex;
    return ch;
}

static void processMouseMove(BRS_GUI_CharTable *charTable, SDL_MouseMotionEvent *motion) {
    GridPosition gridPosition;
    if (calcGridPosition(&gridPosition, charTable, motion->x, motion->y)) {
        uint8_t ch = getCharByGridPosition(&gridPosition);
        charTable->highlightedChar = ch;
    } else {
        charTable->highlightedChar = NO_CHAR;
    }

}

static void processMouseButtonDown(BRS_GUI_CharTable *charTable, SDL_MouseButtonEvent *event) {
    GridPosition gridPosition;
    if (calcGridPosition(&gridPosition, charTable, event->x, event->y)) {
        uint8_t ch = getCharByGridPosition(&gridPosition);
        charTable->selectedChar = ch;
    } else {
        charTable->selectedChar = NO_CHAR;
    }

    if (charTable->clickHandler) {
        charTable->clickHandler(charTable);
    }
}

BRS_GUI_CharTable *
BRS_GUI_CharTable_create(BRS_Point *position, const BRS_Color *borderColor, const BRS_Color *charColor,
                         const BRS_Color *highlightedColor, const BRS_Color *selectedColor, BRS_Font *font) {
    BRS_GUI_CharTable *charTable = malloc(sizeof(BRS_GUI_CharTable));
    charTable->position = BRS_copyPoint(position);
    charTable->borderColor = borderColor;
    charTable->charColor = charColor;
    charTable->highlightedColor = highlightedColor;
    charTable->selectedColor = selectedColor;
    charTable->font = font;
    charTable->highlightedChar = NO_CHAR;
    charTable->selectedChar = NO_CHAR;
    charTable->clickHandler = NULL;
    return charTable;
}

void BRS_GUI_CharTable_destroy(BRS_GUI_CharTable *charTable) {
    free(charTable->position);
    free(charTable);
}

static void drawTableBorder(BRS_VideoContext *context, BRS_GUI_CharTable *charTable) {
    BRS_setColor(context, charTable->borderColor);
    BRS_Rect rect = {
            .x = charTable->position->x,
            .y = charTable->position->y,
            .width = calcTableWith(charTable),
            .height = calcTableHeight(charTable)
    };
    BRS_drawlRect(context, &rect);
}

static void drawVerticalLines(const BRS_VideoContext *context, const BRS_GUI_CharTable *charTable) {
    BRS_setColor(context, charTable->borderColor);

    uint32_t i;
    BRS_Point p1 = {.x = charTable->position->x, .y = charTable->position->y};
    BRS_Point p2 = {.x = charTable->position->x, .y = charTable->position->y +
                                                      CHAR_LINES * (charTable->font->height_bits + PIXELS_BORDER * 2 +
                                                                    PIXELS_PADDING * 2)};
    BRS_Line line = {.p1 = &p1, .p2 = &p2};
    int32_t x_diff = charTable->font->width_bits + +PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
    for (i = 0; i < CHARS_PER_ROW; i++) {
        BRS_drawline(context, &line);
        line.p1->x += x_diff;
        line.p2->x += x_diff;
    }
}

static void drawHorizontalLines(const BRS_VideoContext *context, const BRS_GUI_CharTable *charTable) {
    BRS_setColor(context, charTable->borderColor);

    uint32_t i;
    BRS_Point p1 = {.x = charTable->position->x, .y = charTable->position->y};
    BRS_Point p2 = {.x = charTable->position->x +
                         CHARS_PER_ROW * (charTable->font->width_bits + PIXELS_BORDER * 2 +
                                          PIXELS_PADDING * 2), .y = charTable->position->y};
    BRS_Line line = {.p1 = &p1, .p2 = &p2};
    int32_t y_diff = charTable->font->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
    for (i = 0; i < CHAR_LINES; i++) {
        BRS_drawline(context, &line);
        line.p1->y += y_diff;
        line.p2->y += y_diff;
    }
}


static void drawChars(const BRS_VideoContext *context, const BRS_GUI_CharTable *charTable) {
    BRS_Point pos = {.x = charTable->position->x + PIXELS_BORDER + PIXELS_PADDING, .y = charTable->position->y +
                                                                                        PIXELS_BORDER + PIXELS_PADDING};
    BRS_Rect charRect = {.x = 0, .y = 0,
            .width = charTable->font->width_bits + 2 * PIXELS_PADDING,
            .height = charTable->font->height_bits + 2 * PIXELS_PADDING
    };
    int32_t x_diff = charTable->font->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
    int32_t y_diff = charTable->font->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;

    uint16_t rowIndex;
    uint16_t colIndex;

    int32_t ch = 0;
    uint8_t str[2];
    for (rowIndex = 0; rowIndex < CHAR_LINES; rowIndex++) {
        pos.x = charTable->position->x + PIXELS_BORDER + PIXELS_PADDING;
        for (colIndex = 0; colIndex < CHARS_PER_ROW; colIndex++) {
            if (charTable->highlightedChar == ch) {
                BRS_setColor(context, charTable->highlightedColor);
                charRect.x = pos.x - PIXELS_PADDING;
                charRect.y = pos.y - PIXELS_PADDING;
                BRS_drawlFillRect(context, &charRect);
            }

            str[0] = ch;
            BRS_drawString(context, str, charTable->font, &pos, charTable->charColor);
            pos.x += x_diff;
            ch++;
        }
        pos.y += y_diff;
    }
}

void BRS_GUI_CharTable_render(BRS_VideoContext *context, BRS_GUI_CharTable *charTable) {
    drawTableBorder(context, charTable);
    drawVerticalLines(context, charTable);
    drawHorizontalLines(context, charTable);
    drawChars(context, charTable);
}

void BRS_GUI_CharTable_processEvent(BRS_GUI_CharTable *charTable, SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEMOTION: {
            BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
            BRS_Rect widgetRect = {.x = charTable->position->x, .y = charTable->position->y, .width = calcTableWith(
                    charTable), .height = calcTableHeight(charTable)};
            if (BRS_PointInRect(&mousePoint, &widgetRect)) {
                processMouseMove(charTable, &event->motion);
            } else {
                charTable->highlightedChar = NO_CHAR;
            }

        }
            break;
        case SDL_MOUSEBUTTONUP: {
            BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
            BRS_Rect widgetRect = {.x = charTable->position->x, .y = charTable->position->y, .width = calcTableWith(
                    charTable), .height = calcTableHeight(charTable)};
            if (BRS_PointInRect(&mousePoint, &widgetRect)) {
                processMouseButtonDown(charTable, &event->button);
            } else {
                charTable->selectedChar = NO_CHAR;
            }
        }
            break;
    }
}

void BRS_GUI_CharTable_setClickHandler(BRS_GUI_CharTable *charTable, BRS_GUI_CharTable_ClickHandler handler) {
    charTable->clickHandler = handler;
}
