//
// Created by bernd on 23.01.21.
//

#ifndef FONTEDIT_RENDER_H
#define FONTEDIT_RENDER_H

#include <stdint.h>
#include "video.h"
#include "color.h"

typedef struct _BRS_Size {
    int32_t width;
    int32_t height;
} BRS_Size;

typedef struct _BRS_Point {
    int x;
    int y;
} BRS_Point;

typedef struct _BRS_Line {
    BRS_Point *p1;
    BRS_Point *p2;
} BRS_Line;

typedef struct _BRS_Rect {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} BRS_Rect;

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color);

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point);

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line);

void BRS_drawlRect(const BRS_VideoContext *context, const BRS_Rect *rect);

void BRS_drawlFillRect(const BRS_VideoContext *context, const BRS_Rect *rect);

void BRS_drawString(const BRS_VideoContext *context, const uint8_t *str, uint16_t strlen, const BRS_Font *font,
                    const BRS_Point *startPoint);

bool BRS_PointInRect(BRS_Point *point, BRS_Rect *rect);

BRS_Point *BRS_copyPoint(BRS_Point *source);

BRS_Size *BRS_copySize(BRS_Size *source);

#endif //FONTEDIT_RENDER_H
