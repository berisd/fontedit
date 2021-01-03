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
    BRS_GUI_WidgetList *widgets;
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
    BRS_GUI_WidgetListEntry *entry = applicationState->widgets->firstEntry;
    while (entry != NULL) {
        BRS_GUI_destroyWidget(entry->value);
        BRS_GUI_WidgetListEntry *tmp = entry;
        entry = entry->next;
        free(tmp);
    }
    free(applicationState);
}

static BRS_GUI_Menu *createFileMenu(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_MenuItem *menuItemNew = malloc(sizeof(BRS_GUI_MenuItem));
    menuItemNew->label = "New";

    BRS_GUI_Menu *menu = malloc(sizeof(BRS_GUI_Menu));
    menu->label = "File";
    menu->font = menuBar->font;
    menu->itemList = BRS_GUI_MenuItemList_create();
    menu->color = &COLOR_WHITE;
    BRS_GUI_MenuItemList_push(menuItemNew, menu->itemList);
    return menu;
}

static BRS_GUI_Widget *createMenuBar(ApplicationState *applicationState) {
    BRS_Point *menuBarPosition = malloc(sizeof(BRS_Point));
    menuBarPosition->x = 0;
    menuBarPosition->y = 0;
    BRS_Dimension *menuBarDimension = malloc(sizeof(struct BRS_Dimension));
    menuBarDimension->width = 800;
    menuBarDimension->height = 20;
    BRS_GUI_Widget *widget = BRS_GUI_createMenuBar(menuBarPosition, menuBarDimension, &COLOR_BLUE,
                                                     applicationState->defaultFont);
    BRS_GUI_MenuList_push(createFileMenu(widget->object->menuBar), widget->object->menuBar->menuList);
    return widget;
}

static BRS_GUI_WidgetList *createWidgets(ApplicationState *applicationState) {
    BRS_GUI_WidgetList *list = BRS_GUI_WidgetList_create();

    BRS_Point *labelPosition = malloc(sizeof(BRS_Point));
    labelPosition->x = 100;
    labelPosition->y = 150;

    BRS_GUI_WidgetList_push(
            BRS_GUI_createLabel(labelPosition, &COLOR_RED, "Hello World", applicationState->defaultFont), list);
    BRS_GUI_WidgetList_push(createMenuBar(applicationState), list);

    return list;
}

static ApplicationState *initApplication(const ApplicationConfig *config) {
    ApplicationState *applicationState = createApplicationState();
    BRS_VideoContext *videoContext = BRS_initVideo(config->screenWidth, config->screenHeight);
    if (videoContext == NULL) {
        return NULL;
    }
    applicationState->videoContext = videoContext;
    applicationState->defaultFont = createDefaultFont();
    applicationState->widgets = createWidgets(applicationState);
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

static void handleVideo(ApplicationState *applicationState) {
    BRS_setColor(applicationState->videoContext, &COLOR_BLACK);
    BRS_clearVideo(applicationState->videoContext);

    BRS_GUI_WidgetListEntry *listEnty = applicationState->widgets->firstEntry;
    while (listEnty != NULL) {
        BRS_GUI_renderWidget(applicationState->videoContext, listEnty->value);
        listEnty = listEnty->next;
    }

    BRS_updateVideo(applicationState->videoContext);
}

static void runApplication(ApplicationState *applicationState) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        if (SDL_PollEvent(&event) != 0) {
            enum APP_ACTION action = processEvent(event);
            if (action != NONE) {
                quit = true;
                continue;
            }
        }
        handleVideo(applicationState);
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
