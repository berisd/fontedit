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
    BRS_Font *defaultFont;
    BRS_GUI_WidgetList *widgets;
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
    BRS_destroyFont(applicationState->defaultFont);
    destroyWidgets(applicationState->widgets);
    free(applicationState);
}

static ApplicationState *initApplication(const ApplicationConfig *config) {
    ApplicationState *applicationState = createApplicationState();
    BRS_VideoContext *videoContext = BRS_initVideo(config->screenWidth, config->screenHeight);
    if (videoContext == NULL) {
        return NULL;
    }
    applicationState->videoContext = videoContext;
    applicationState->defaultFont = createDefaultFont();
    applicationState->fontEdited = BRS_copyFont(applicationState->defaultFont);
    applicationState->widgets = createWidgets(applicationState->defaultFont, config->screenWidth, config->screenHeight);
    applicationState->quit = false;
    atexit(SDL_Quit);
    return applicationState;
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

static void processEvent(SDL_Event *event, ApplicationState *applicationState) {
    BRS_GUI_WidgetList *widgetList = applicationState->widgets;
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget_processEvent(entry->value, event);
        entry = entry->next;
    }
}

static void handleVideo(ApplicationState *applicationState) {
    BRS_setColor(applicationState->videoContext, &COLOR_BLACK);
    BRS_clearVideo(applicationState->videoContext);

    BRS_GUI_WidgetListEntry *listEntry = applicationState->widgets->firstEntry;
    while (listEntry != NULL) {
        BRS_GUI_Widget_render(applicationState->videoContext, listEntry->value);
        listEntry = listEntry->next;
    }

    BRS_updateVideo(applicationState->videoContext);
}

static void runApplication(ApplicationState *applicationState) {
    SDL_Event event;
    while (!applicationState->quit) {
        if (SDL_PollEvent(&event) != 0) {
            applicationState->quit = checkQuitApplication(&event);
            processEvent(&event, applicationState);
        }
        handleVideo(applicationState);
    }
}

void quitApplication() {
    applicationState->quit = true;
}

BRS_GUI_WidgetList *getWidgetList() {
    return applicationState->widgets;
}

int main() {
    ApplicationConfig *config = createConfig();
    applicationState = initApplication(config);
    if (applicationState != NULL) {
        runApplication(applicationState);
    }
    shutdownApplication(applicationState);
    destroyConfig(config);
    return EXIT_SUCCESS;
}
