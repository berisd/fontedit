//
// Created by bernd on 02.01.21.
//

#ifndef FONTEDIT_VIDEO_H
#define FONTEDIT_VIDEO_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "font.h"

typedef struct BRS_VideoContext {
    uint32_t screenWidth;
    uint32_t screenHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
} BRS_VideoContext;

BRS_VideoContext *BRS_initVideo(uint32_t screenWidth, uint32_t screenHeight);

void BRS_shutdownVideo(const BRS_VideoContext *context);

void BRS_clearVideo(const BRS_VideoContext *context);

void BRS_updateVideo(const BRS_VideoContext *context);

#endif //FONTEDIT_VIDEO_H
