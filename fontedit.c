#include <stdio.h>
#include <stdbool.h>
#include "video.h"
#include "font.h"
#include "gui.h"

typedef struct _BRS_ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} BRS_ApplicationConfig;

typedef struct _BRS_ApplicationState {
    BRS_VideoContext *videoContext;
    BRS_GUI_Theme *theme;
    BRS_GUI *gui;
    BRS_Font *fontEdited;
    bool quit;
} BRS_ApplicationState;

static BRS_ApplicationState *applicationState = NULL;

static BRS_ApplicationConfig *createConfig() {
    BRS_ApplicationConfig *config = malloc(sizeof(BRS_ApplicationConfig));
    config->screenWidth = 800;
    config->screenHeight = 600;
    return config;
}

static void destroyConfig(BRS_ApplicationConfig *config) {
    free(config);
}

static BRS_ApplicationState *createApplicationState() {
    BRS_ApplicationState *applicationState = malloc(sizeof(BRS_ApplicationState));
    return applicationState;
}

static void freeApplicationState(BRS_ApplicationState *applicationState) {
    BRS_GUI_Theme_destroy(applicationState->theme);
    BRS_GUI_destroyGUI(applicationState->gui);
    free(applicationState);
}

static void *initApplication(const BRS_ApplicationConfig *config) {
    applicationState = createApplicationState();
    BRS_VideoContext *videoContext = BRS_initVideo(config->screenWidth, config->screenHeight);
    if (videoContext == NULL) {
        return NULL;
    }
    applicationState->videoContext = videoContext;
    applicationState->theme = BRS_GUI_Theme_create();
    applicationState->fontEdited = BRS_copyFont(applicationState->theme->font);
    applicationState->gui = BRS_GUI_createGUI(applicationState->theme, applicationState->fontEdited,
                                              config->screenWidth, config->screenHeight);
    applicationState->quit = false;
    atexit(SDL_Quit);

    BRS_GUI_initGUI(applicationState->gui);
}

static void shutdownApplication(BRS_ApplicationState *applicationState) {
    BRS_shutdownVideo(applicationState->videoContext);
    freeApplicationState(applicationState);
}

static bool checkQuitApplication(SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_ESCAPE:
                    return true;
            }
    }
    return false;
}

static void handleVideo(BRS_ApplicationState *applicationState) {
    BRS_setColor(applicationState->videoContext, &COLOR_BLACK);
    BRS_clearVideo(applicationState->videoContext);
    BRS_GUI_renderGUI(applicationState->videoContext, applicationState->gui);
    BRS_updateVideo(applicationState->videoContext);
}

static void runApplication(BRS_ApplicationState *applicationState) {
    SDL_Event event;
    while (!applicationState->quit) {
        if (SDL_PollEvent(&event) != 0) {
            applicationState->quit = checkQuitApplication(&event);
            BRS_GUI_processEvent(applicationState->gui, &event);
        }
        handleVideo(applicationState);
    }
}

void BRS_FontEdit_quitApplication() {
    applicationState->quit = true;
}

BRS_GUI_WidgetList *BRS_FontEdit_getWidgetList() {
    return applicationState->gui;
}

int main() {
    BRS_ApplicationConfig *config = createConfig();
    initApplication(config);
    if (applicationState != NULL) {
        runApplication(applicationState);
    }
    shutdownApplication(applicationState);
    destroyConfig(config);
    return EXIT_SUCCESS;
}
