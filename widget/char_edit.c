//
// Created by bernd on 07.01.21.
//

#include "char_edit.h"

static const int32_t PIXELS_PER_DOT = 16;
static const int32_t PIXELS_PADDING = 1;
static const int32_t PIXELS_BORDER = 1;
static const int32_t NO_CHAR = -1;
static const uint8_t NO_BUTTON = 0;

static int32_t calcCellWidth() {
    return PIXELS_PER_DOT + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
}

static int32_t calcCellHeight() {
    return PIXELS_PER_DOT + PIXELS_BORDER * 2 + PIXELS_PADDING * 2;
}

static int32_t calcTableWith(BRS_GUI_CharEdit *charEdit) {
    return charEdit->fontEdited->width_bits * calcCellWidth();
}

static int32_t calcTableHeight(BRS_GUI_CharEdit *charEdit) {
    return charEdit->fontEdited->height_bits * calcCellHeight();
}

static void calcGridPosition(GridPosition *gridPosition, BRS_GUI_Widget *widget, BRS_Point *mousePoint) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    int32_t cellWidth = calcCellWidth();
    int32_t cellHeight = calcCellHeight();

    int32_t relX = mousePoint->x - widgetProps->position->x;
    int32_t relY = mousePoint->y - widgetProps->position->y;

    gridPosition->columnIndex = relX / cellWidth;
    gridPosition->rowIndex = relY / cellHeight;
}

BRS_GUI_CharEdit *
BRS_GUI_CharEdit_create(BRS_Font *fontEdited) {
    BRS_GUI_CharEdit *charEdit = malloc(sizeof(BRS_GUI_CharEdit));
    charEdit->fontEdited = fontEdited;
    charEdit->selectedChar = NO_CHAR;
    charEdit->_buttonPressed = NO_BUTTON;
    return charEdit;
}

void BRS_GUI_CharEdit_destroy(BRS_GUI_CharEdit *charEdit) {
    free(charEdit);
}

static void drawTableBorder(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_CharEdit *charEdit = widget->object;
    BRS_setColor(context, widgetProps->theme->charEditForeColor);
    BRS_Rect rect = {
            .x = widgetProps->position->x,
            .y = widgetProps->position->y,
            .width = charEdit->fontEdited->width_bits * (PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2),
            .height = charEdit->fontEdited->height_bits * (PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2)
    };
    BRS_drawlRect(context, &rect);
}

static void drawVerticalLines(const BRS_VideoContext *context, const BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_CharEdit *charEdit = widget->object;
    uint32_t i;
    BRS_Point p1 = {.x = widgetProps->position->x, .y = widgetProps->position->y};
    BRS_Point p2 = {.x = widgetProps->position->x, .y = widgetProps->position->y +
                                                        charEdit->fontEdited->height_bits *
                                                        (PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2)};
    BRS_Line line = {.p1 = &p1, .p2 = &p2};
    int32_t x_diff = PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2;
    BRS_setColor(context, widgetProps->theme->charEditForeColor);
    for (i = 0; i < charEdit->fontEdited->width_bits; i++) {
        BRS_drawline(context, &line);
        line.p1->x += x_diff;
        line.p2->x += x_diff;
    }
}

static void drawHorizontalLines(const BRS_VideoContext *context, const BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_CharEdit *charEdit = widget->object;
    uint32_t i;
    BRS_Point p1 = {.x = widgetProps->position->x, .y = widgetProps->position->y};
    BRS_Point p2 = {.x = widgetProps->position->x +
                         charEdit->fontEdited->width_bits *
                         (PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2), .y = widgetProps->position->y};
    BRS_Line line = {.p1 = &p1, .p2 = &p2};
    int32_t y_diff = PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2;
    BRS_setColor(context, widgetProps->theme->charEditForeColor);
    for (i = 0; i < charEdit->fontEdited->height_bits; i++) {
        BRS_drawline(context, &line);
        line.p1->y += y_diff;
        line.p2->y += y_diff;
    }
}

static void drawDotsForChar(const BRS_VideoContext *context, const BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_CharEdit *charEdit = widget->object;
    BRS_Font *font = charEdit->fontEdited;
    int32_t ch = charEdit->selectedChar;
    int16_t fontCharPos = ch * font->height_bits;
    BRS_Rect rect = {.x= widgetProps->position->x, .y=widgetProps->position->y, .width=PIXELS_PER_DOT, .height=PIXELS_PER_DOT};
    BRS_setColor(context, widgetProps->theme->charEditDotColor);
    for (int fontCharByteCounter = 0; fontCharByteCounter < font->height_bits; fontCharByteCounter++) {
        uint8_t fontCharByte = font->data[fontCharPos + fontCharByteCounter];
        for (int fontCharBitCounter = 0; fontCharBitCounter < font->width_bits; fontCharBitCounter++) {
            uint8_t bitValue = 128 >> fontCharBitCounter;
            if (fontCharByte & bitValue) {
                BRS_Rect fillRect = {.x = rect.x + PIXELS_BORDER + PIXELS_PADDING, .y = rect.y + PIXELS_BORDER +
                                                                                        PIXELS_PADDING, .width = rect.width, .height = rect.height};
                BRS_drawlFillRect(context, &fillRect);
            }
            rect.x += PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2;
        }
        rect.y += PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2;
        rect.x = widgetProps->position->x;
    }
}

static void clearTable(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_CharEdit *charEdit = widget->object;
    BRS_setColor(context, widgetProps->theme->screenColor);
    BRS_Rect rect = {
            .x = widgetProps->position->x,
            .y = widgetProps->position->y,
            .width = charEdit->fontEdited->width_bits * (PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2),
            .height = charEdit->fontEdited->height_bits * (PIXELS_PER_DOT + PIXELS_PADDING * 2 + PIXELS_BORDER * 2)
    };
    BRS_drawlFillRect(context, &rect);
}

void BRS_GUI_CharEdit_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_GUI_CharEdit *charEdit = widget->object;
    clearTable(context, widget);
    drawTableBorder(context, widget);
    drawVerticalLines(context, widget);
    drawHorizontalLines(context, widget);
    if (charEdit->selectedChar != NO_CHAR) {
        drawDotsForChar(context, widget);
    }
}

static void setCharDot(BRS_GUI_Widget *widget, GridPosition *gridPosition) {
    BRS_GUI_CharEdit *charEdit = widget->object;
    if (charEdit->_buttonPressed == NO_BUTTON) {
        return;
    }

    BRS_Font *font = charEdit->fontEdited;
    int32_t ch = charEdit->selectedChar;

    int32_t charRowBytePos = ch * font->height_bits + gridPosition->rowIndex;
    int8_t charRowByte = font->data[charRowBytePos];

    if (charEdit->_buttonPressed == SDL_BUTTON_LEFT) {
        charRowByte |= (128 >> gridPosition->columnIndex);
    } else {
        charRowByte &= ~(128 >> gridPosition->columnIndex);
    }
    font->data[charRowBytePos] = charRowByte;
}

static void setCharDotAtPoint(BRS_GUI_Widget *widget, BRS_Point *mousePoint) {
    GridPosition gridPosition;
    calcGridPosition(&gridPosition, widget, mousePoint);
    setCharDot(widget, &gridPosition);
}

bool BRS_GUI_CharEdit_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_CharEdit *charEdit = widget->object;
    BRS_Rect widgetRect = {.x = widgetProps->position->x, .y = widgetProps->position->y, .width = calcTableWith(
            charEdit), .height = calcTableHeight(charEdit)};

    switch (event->type) {
        case SDL_MOUSEMOTION: {
            BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
            if (BRS_PointInRect(&mousePoint, &widgetRect)) {
                setCharDotAtPoint(widget, &mousePoint);
            }
        }
            break;
        case SDL_MOUSEBUTTONDOWN: {
            BRS_Point mousePoint = {.x = event->button.x, .y = event->button.y};
            if (BRS_PointInRect(&mousePoint, &widgetRect)) {
                charEdit->_buttonPressed = event->button.button;
                setCharDotAtPoint(widget, &mousePoint);
            } else {
                charEdit->_buttonPressed = NO_BUTTON;
            }
        }
            break;
        case SDL_MOUSEBUTTONUP:
            charEdit->_buttonPressed = NO_BUTTON;
            break;
    }
    return false;
}