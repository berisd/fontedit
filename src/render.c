//
// Created by bernd on 23.01.21.
//

#include "render.h"

static const uint8_t PIXELS_BETWEEN_CHARS = 1;

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color) {
    SDL_SetRenderDrawColor(context->renderer, color->r, color->g, color->b, color->a);
}

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point) {
    SDL_RenderDrawPoint(context->renderer, point->x, point->y);
}

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line) {
    SDL_RenderDrawLine(context->renderer, line->p1->x, line->p1->y, line->p2->x, line->p2->y);
}

void BRS_drawlRect(const BRS_VideoContext *context, const BRS_Rect *rect) {
    SDL_Rect sdlRect = {.x = rect->x, .y = rect->y, .w = rect->width, .h = rect->height};
    SDL_RenderDrawRect(context->renderer, &sdlRect);
}

void BRS_drawlFillRect(const BRS_VideoContext *context, const BRS_Rect *rect) {
    SDL_Rect sdlRect = {.x = rect->x, .y = rect->y, .w = rect->width, .h = rect->height};
    SDL_RenderFillRect(context->renderer, &sdlRect);
}

void BRS_drawString(const BRS_VideoContext *context, const uint8_t *str, uint16_t strlen, const BRS_Font *font,
                    const BRS_Point *startPoint) {
    int32_t x = startPoint->x;
    int32_t y = startPoint->y;
    BRS_Point point;
    for (int i = 0; i < strlen; i++) {
        uint8_t strChar = str[i];
        int16_t fontCharPos = strChar * font->height_bits;
        for (int fontCharByteCounter = 0; fontCharByteCounter < font->height_bits; fontCharByteCounter++) {
            uint8_t fontCharByte = font->data[fontCharPos + fontCharByteCounter];
            for (int fontCharBitCounter = 0; fontCharBitCounter < font->width_bits; fontCharBitCounter++) {
                uint8_t bitValue = 128 >> fontCharBitCounter;
                if (fontCharByte & bitValue) {
                    point.x = x;
                    point.y = y;
                    BRS_drawPoint(context, &point);
                }
                x++;
            }
            y++;
            x -= font->width_bits;
        }
        x += PIXELS_BETWEEN_CHARS;
        x += font->width_bits;
        y -= font->height_bits;
    }
}

bool BRS_PointInRect(BRS_Point *point, BRS_Rect *rect) {
    bool intersect = (point->x >= rect->x && point->x <= rect->x + rect->width &&
                      point->y >= rect->y && point->y <= rect->y + rect->height);
    return intersect;
}

BRS_Point *BRS_copyPoint(BRS_Point *source) {
    BRS_Point *copy = malloc(sizeof(source));
    memcpy(copy, source, sizeof(source));
    return copy;
}

BRS_Size *BRS_copySize(BRS_Size *source) {
    BRS_Size *copy = malloc(sizeof(source));
    memcpy(copy, source, sizeof(source));
    return copy;
}

BRS_Padding *BRS_copyPadding(BRS_Padding *source) {
    BRS_Padding *copy = malloc(sizeof(source));
    memcpy(copy, source, sizeof(source));
    return copy;
}
