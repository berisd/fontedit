//
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
#include "video.h"

const BRS_Color COLOR_RED = {.r = 255, .g = 0, .b = 0, .a = 255};
const BRS_Color COLOR_BLUE = {.r = 0, .g = 0, .b = 255, .a = 255};
const BRS_Color COLOR_YELLOW = {.r = 255, .g = 255, .b = 0, .a = 255};
const BRS_Color COLOR_WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
const BRS_Color COLOR_BLACK = {.r = 0, .g = 0, .b = 0, .a = 0};

static const uint8_t PIXELS_BETWEEN_CHARS = 1;

static BRS_VideoContext *createVideoContext(int32_t screenWidth, int32_t screenHeight) {
    BRS_VideoContext *context = malloc(sizeof(BRS_VideoContext));
    context->window = NULL;
    context->renderer = NULL;
    context->screenWidth = screenWidth;
    context->screenHeight = screenHeight;
    return context;
}

static bool initSdl(BRS_VideoContext *videoContext) {
    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using SDL version %d.%d.%d", sdlVersion.major, sdlVersion.minor,
                sdlVersion.patch);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    videoContext->window = SDL_CreateWindow("Fontedit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            videoContext->screenWidth,
                                            videoContext->screenHeight,
                                            SDL_WINDOW_SHOWN);

    if (videoContext->window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    videoContext->renderer = SDL_CreateRenderer(videoContext->window, -1, SDL_RENDERER_PRESENTVSYNC);

    if (videoContext->renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    return true;
}

BRS_VideoContext *BRS_initVideo(uint32_t screenWidth, uint32_t screenHeight) {
    BRS_VideoContext *context = createVideoContext(screenWidth, screenHeight);
    if (!initSdl(context)) {
        free(context);
        return NULL;
    }
    return context;
}

void BRS_shutdownVideo(const BRS_VideoContext *context) {
    SDL_DestroyRenderer(context->renderer);
    SDL_DestroyWindow(context->window);
    free((BRS_VideoContext *) context);
}

void BRS_clearVideo(const BRS_VideoContext *context) {
    SDL_RenderClear(context->renderer);
}

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color) {
    SDL_SetRenderDrawColor(context->renderer, color->r, color->g, color->b, color->a);
}

void BRS_updateVideo(const BRS_VideoContext *context) {
    SDL_RenderPresent(context->renderer);
}

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point) {
    SDL_RenderDrawPoint(context->renderer, point->x, point->y);
}

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line) {
    SDL_RenderDrawLine(context->renderer, line->p1->x, line->p1->y, line->p2->x, line->p2->y);
}

void BRS_drawString(const BRS_VideoContext *context, const char *str, const BRS_Font *font, const BRS_Point *startPoint,
                    const BRS_Color *color) {
    BRS_setColor(context, color);
    int32_t x = startPoint->x;
    int32_t y = startPoint->y;
    size_t len = strlen(str);
    BRS_Point point;
    for (int i = 0; i < len; i++) {
        char strChar = str[i];
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
