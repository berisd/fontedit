//
// Created by bernd on 02.01.21.
//

#ifndef FONTEDIT_VIDEO_H
#define FONTEDIT_VIDEO_H

#include <SDL2/SDL.h>
#include "font.h"

typedef struct BRS_VideoContext {
    uint32_t screenWidth;
    uint32_t screenHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
} BRS_VideoContext;

struct BRS_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
typedef struct BRS_Color BRS_Color;

typedef struct BRS_Point {
    int x;
    int y;
} BRS_Point;
typedef struct BRS_Point BRS_Point;

struct BRS_Line {
    BRS_Point *p1;
    BRS_Point *p2;
};
typedef struct BRS_Line BRS_Line;

struct BRS_Dimension {
    int32_t width;
    int32_t height;
};
typedef struct BRS_Dimension BRS_Dimension;

struct BRS_Rect {
    BRS_Point position;
    BRS_Dimension dimension;
};
typedef struct BRS_Rect BRS_Rect;

const BRS_Color COLOR_RED;
const BRS_Color COLOR_BLUE;
const BRS_Color COLOR_YELLOW;
const BRS_Color COLOR_WHITE;
const BRS_Color COLOR_BLACK;

BRS_VideoContext *BRS_initVideo(uint32_t screenWidth, uint32_t screenHeight);

void BRS_shutdownVideo(const BRS_VideoContext *context);

void BRS_clearVideo(const BRS_VideoContext *context);

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color);

void BRS_updateVideo(const BRS_VideoContext *context);

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point);

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line);

void BRS_drawString(const BRS_VideoContext *context, const char *str, const BRS_Font *font, const BRS_Point *startPoint,
                    const BRS_Color *color);

BRS_Point *BRS_copyPoint(BRS_Point *source);

BRS_Dimension *BRS_copyDimension(BRS_Dimension *source);

#endif //FONTEDIT_VIDEO_H
