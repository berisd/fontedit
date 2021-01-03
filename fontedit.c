#include <stdio.h>
#include <stdbool.h>
#include "video.h"
#include "font.h"
#include "util.h"
#include "widget.h"

typedef struct ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} ApplicationConfig;

typedef struct ApplicationState {
    BRS_VideoContext *videoContext;
    BRS_Font *defaultFont;
    BRS_GUI_MenuBar *menuBar;
} ApplicationState;

typedef enum APP_ACTION {
    NONE, QUIT_APP
} APP_ACTION;

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
    BRS_freeFont(applicationState->defaultFont);
    free(applicationState);
}

static BRS_GUI_Menu *createFileMenu() {
    BRS_GUI_MenuItem *menuItemNew = malloc(sizeof(BRS_GUI_MenuItem));
    menuItemNew->label = "New";

    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->label = "File";
    menu->itemList = BRS_GUI_MenuItemList_create();
    BRS_GUI_MenuItemList_push(menuItemNew, menu->itemList);
    return menu;
}

static BRS_GUI_MenuBar *createMenuBar() {
    BRS_Point *menuBarPosition = malloc(sizeof(BRS_Point));
    menuBarPosition->x = 0;
    menuBarPosition->y = 0;
    BRS_Dimension *menuBarDimension = malloc(sizeof(struct BRS_Dimension));
    menuBarDimension->width = 800;
    menuBarDimension->height = 20;
    BRS_GUI_MenuBar *menuBar = BRS_GUI_createMenuBar(menuBarPosition, menuBarDimension, &COLOR_BLUE);
    BRS_GUI_MenuList_push(createFileMenu(), menuBar->menuList);
    return menuBar;
}

static ApplicationState *initApplication(const ApplicationConfig *config) {
    ApplicationState *applicationState = createApplicationState();
    BRS_VideoContext *videoContext = BRS_initVideo(config->screenWidth, config->screenHeight);
    if (videoContext == NULL) {
        return NULL;
    }
    applicationState->videoContext = videoContext;
    applicationState->defaultFont = createDefaultFont();
    applicationState->menuBar = createMenuBar();
    atexit(SDL_Quit);
    return applicationState;
}

static void shutdownApplication(ApplicationState *applicationState) {
    BRS_shutdownVideo(applicationState->videoContext);
    freeApplicationState(applicationState);
}

static APP_ACTION processEvent(SDL_Event event) {
    if (event.type == SDL_QUIT) {
        return QUIT_APP;
    } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return QUIT_APP;
            default:
                break;
        }
    }
    return NONE;
}

static void runApplication(ApplicationState *applicationState) {
    bool quit = false;
    SDL_Event event;

    BRS_Point labelStart = {.x = 100, .y= 150};

    while (!quit) {

        if (SDL_PollEvent(&event) != 0) {
            enum APP_ACTION action = processEvent(event);
            if (action != NONE) {
                quit = true;
                continue;
            }
        }
        BRS_setColor(applicationState->videoContext, &COLOR_BLACK);
        BRS_clearVideo(applicationState->videoContext);

        BRS_GUI_renderMenuBar(applicationState->videoContext, applicationState->menuBar, applicationState->defaultFont);

        BRS_drawString(applicationState->videoContext, "Hello World!", applicationState->defaultFont, &labelStart,
                       &COLOR_RED);

        BRS_updateVideo(applicationState->videoContext);
    }
}

int main() {
    ApplicationConfig *config = createConfig();
    ApplicationState *applicationState = initApplication(config);
    if (applicationState != NULL) {
        runApplication(applicationState);
    }
    shutdownApplication(applicationState);
    destroyConfig(config);
    return EXIT_SUCCESS;
}
