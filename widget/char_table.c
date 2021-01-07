//
// Created by bernd on 07.01.21.
//

#include "char_table.h"

static const uint16_t CHARS_PER_ROW = 32;
static const uint16_t CHAR_LINES = 8;
static const int32_t PIXELS_PADDING = 2;
static const int32_t PIXELS_BORDER = 1;

BRS_GUI_CharTable *
BRS_GUI_CharTable_create(BRS_Point *position, const BRS_Color *foreColor, BRS_Font *font) {
    BRS_GUI_CharTable *charTable = malloc(sizeof(BRS_GUI_CharTable));
    charTable->position = BRS_copyPoint(position);
    charTable->foreColor = foreColor;
    charTable->font = font;
    return charTable;
}

void BRS_GUI_CharTable_destroy(BRS_GUI_CharTable *charTable) {
    free(charTable->position);
    free(charTable);
}

static void drawTableBorder(BRS_VideoContext *context, BRS_GUI_CharTable *charTable) {
    BRS_setColor(context, charTable->foreColor);
    BRS_Rect rect = {
            .x = charTable->position->x,
            .y = charTable->position->y,
            .width = CHARS_PER_ROW * (charTable->font->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2),
            .height = CHAR_LINES * (charTable->font->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2)
    };
    BRS_drawlRect(context, &rect);
}

static void drawVerticalLines(const BRS_VideoContext *context, const BRS_GUI_CharTable *charTable) {
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
    int32_t x_diff = charTable->font->width_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
    int32_t y_diff = charTable->font->height_bits + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;

    uint16_t rowIndex;
    uint16_t colIndex;

    char ch = 0;
    char str[2];
    for (rowIndex = 0; rowIndex < CHAR_LINES; rowIndex++) {
        pos.x = charTable->position->x + PIXELS_BORDER + PIXELS_PADDING;
        for (colIndex = 0; colIndex < CHARS_PER_ROW; colIndex++) {
            str[0] = ch;
            BRS_drawString(context, str, charTable->font, &pos, charTable->foreColor);
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


