#include <stdio.h>
#include <stdbool.h>
#include "video.h"
#include "font.h"
#include "gui.h"
#include "widget/label.h"


typedef struct _ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} ApplicationConfig;

typedef struct _ApplicationState {
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
    BRS_GUI_destroy(applicationState->gui);
    BRS_Font_destroy(applicationState->fontEdited);
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
    applicationState->fontEdited = BRS_Font_copy(applicationState->theme->font);
    applicationState->gui = BRS_GUI_create(applicationState->theme, applicationState->fontEdited,
                                           config->screenWidth, config->screenHeight);
    applicationState->quit = false;
    atexit(SDL_Quit);

    BRS_GUI_init(applicationState->gui);
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
    BRS_GUI_render(applicationState->videoContext, applicationState->gui);
    BRS_updateVideo(applicationState->videoContext);
}

static void runApplication(ApplicationState *applicationState) {
    SDL_Event event;
    while (!applicationState->quit) {
        BRS_GUI_calculate(applicationState->gui);
        if (SDL_PollEvent(&event) != 0) {
            if (!BRS_GUI_processEvent(applicationState->gui, &event)) {
                if (!applicationState->quit) {
                    applicationState->quit = checkQuitApplication(&event);
                }
            }
        }
        handleVideo(applicationState);
    }
}

void BRS_FontEdit_createFont() {
    memset(applicationState->fontEdited->data, 0, BRS_Font_getSize(applicationState->fontEdited));
}

bool BRS_FontEdit_loadFont(const char *filename) {
    BRS_Font_LoadResult *result = BRS_Font_load(filename);
    bool success = false;
    if (result->error == BRS_FONT_NO_ERROR) {
        BRS_Font_destroy(applicationState->fontEdited);
        applicationState->fontEdited = BRS_Font_copy(result->font);
        success = true;
    } else if (result->error == BRS_FONT_ERR_OPENING_FILE || result->error == BRS_FONT_ERR_READING_FILE) {
        success = false;
    }
    BRS_Font_destroyLoadResult(result);
    return success;
}

bool BRS_FontEdit_saveFont(const char *filename) {
    bool success = false;
    BRS_Font_SaveResult *result = BRS_Font_save(applicationState->fontEdited, filename);
    if (result->error == BRS_FONT_NO_ERROR) {
        success = true;
    } else if (result->error == BRS_FONT_ERR_CREATING_FILE || result->error == BRS_FONT_ERR_WRITING_FILE) {
        success = false;
    }

    BRS_Font_destroySaveResult(result);
    return success;
}

void BRS_FontEdit_quitApplication() {
    applicationState->quit = true;
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
