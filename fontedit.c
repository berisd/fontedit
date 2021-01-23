#include <stdio.h>
#include <stdbool.h>
#include "video.h"
#include "font.h"
#include "util.h"
#include "gui.h"

typedef struct ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} ApplicationConfig;

typedef struct ApplicationState {
    BRS_VideoContext *videoContext;
    BRS_GUI_Theme *theme;
    BRS_GUI *gui;
    BRS_Font *fontEdited;
    bool quit;
} ApplicationState;

static ApplicationState *applicationState = NULL;

static ApplicationConfig *createConfig() {
    ApplicationConfig *config = malloc(sizeof(ApplicationConfig));
    config->screenWidth = 800;
    config->screenHeight = 600;
    return config;
}

static void destroyConfig(ApplicationConfig *config) {
    free(config);
}

static ApplicationState *createApplicationState() {
    ApplicationState *applicationState = malloc(sizeof(ApplicationState));
    return applicationState;
}

static void freeApplicationState(ApplicationState *applicationState) {
    BRS_GUI_Theme_destroy(applicationState->theme);
    BRS_GUI_destroyGUI(applicationState->gui);
    free(applicationState);
}

static void *initApplication(const ApplicationConfig *config) {
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

static void shutdownApplication(ApplicationState *applicationState) {
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

static void handleVideo(ApplicationState *applicationState) {
    BRS_setColor(applicationState->videoContext, &COLOR_BLACK);
    BRS_clearVideo(applicationState->videoContext);
    BRS_GUI_renderGUI(applicationState->videoContext, applicationState->gui);
    BRS_updateVideo(applicationState->videoContext);
}

static void runApplication(ApplicationState *applicationState) {
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

BRS_GUI_WidgetList *getWidgetList() {
    return applicationState->gui;
}

int main() {
    ApplicationConfig *config = createConfig();
    initApplication(config);
    if (applicationState != NULL) {
        runApplication(applicationState);
    }
    shutdownApplication(applicationState);
    destroyConfig(config);
    return EXIT_SUCCESS;
}
