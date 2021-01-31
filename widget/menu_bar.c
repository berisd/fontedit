//
// Created by bernd on 05.01.21.
//

#include "menu_bar.h"

BRS_GUI_MenuBar *
BRS_GUI_MenuBar_create() {
    BRS_GUI_MenuBar *menuBar = malloc(sizeof(BRS_GUI_MenuBar));
    menuBar->menuList = BRS_GUI_MenuList_create();
    return menuBar;
}

void BRS_GUI_MenuBar_destroy(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_MenuListEntry *entry = menuBar->menuList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Menu *menu = entry->value;
        BRS_GUI_Menu_destroy(menu);
        entry = entry->next;
    }
    free(menuBar);
}

static void
BRS_GUI_Menu_calcPosition(BRS_MenuBar_RenderContext *renderContext) {
    BRS_Point *menuBarPos = renderContext->menuBarWidget->position;
    BRS_Size *menuSize = renderContext->menu->size;
    renderContext->menu->position->x = menuBarPos->x + menuSize->width * renderContext->menuIndex;
    renderContext->menu->position->y = menuBarPos->y;
}

static void calculateMenuItemPosition(BRS_MenuBar_RenderContext *renderContext) {
    BRS_Point *menuPosition = renderContext->menu->position;
    BRS_Point *menuItemPosition = renderContext->menuItem->position;
    BRS_Size *menuSize = renderContext->menu->size;

    menuItemPosition->x = menuPosition->x;
    menuItemPosition->y = menuPosition->y + menuSize->height + menuSize->height * renderContext->menuItemIndex;
}


static void BRS_GUI_MenuItem_render(BRS_MenuBar_RenderContext *renderContext) {
    BRS_GUI_MenuItem *menuItem = renderContext->menuItem;
    BRS_Point *position = renderContext->menuItem->position;

    const BRS_GUI_Theme *theme = renderContext->menuBarWidget->theme;
    BRS_setColor(renderContext->videoContext, theme->menuItemBackColor);
    BRS_Rect menuItemRect = {.x = position->x, .y = position->y, .width=menuItem->size->width, .height=menuItem->size->height};
    BRS_drawlFillRect(renderContext->videoContext, &menuItemRect);

    BRS_setColor(renderContext->videoContext,
                 menuItem->highlighted ? theme->menuItemHighlightedColor
                                       : theme->menuItemForeColor);
    BRS_drawString(renderContext->videoContext, menuItem->label, strlen(menuItem->label),
                   theme->font, position);
}

static void BRS_MenuItemConsumer_render(BRS_GUI_MenuItem *menuItem, void *context) {
    BRS_MenuBar_RenderContext *ctx = context;
    ctx->menuItem = menuItem;
    BRS_GUI_MenuItem_render(ctx);
    ctx->menuItemIndex++;
}

static void
BRS_GUI_Menu_render(BRS_MenuBar_RenderContext *renderContext) {
    BRS_Point *position = renderContext->menu->position;
    BRS_GUI_Widget *menuBarWidget = renderContext->menuBarWidget;
    BRS_setColor(renderContext->videoContext, menuBarWidget->theme->menuBackColor);
    BRS_Rect menuRect = {.x = position->x, .y = position->y, .width = renderContext->menu->size->width, .height = renderContext->menu->size->height};
    BRS_drawlFillRect(renderContext->videoContext, &menuRect);

    BRS_setColor(renderContext->videoContext,
                 renderContext->menu->selected ? menuBarWidget->theme->menuSelectedForeColor
                                               : menuBarWidget->theme->menuForeColor);
    BRS_drawString(renderContext->videoContext, renderContext->menu->label, strlen(renderContext->menu->label),
                   menuBarWidget->theme->font, position);

    if (renderContext->menu->selected) {
        BRS_GUI_MenuItemList_iterate(renderContext->menu->itemList, BRS_MenuItemConsumer_render, renderContext);
    }
}

static void BRS_MenuConsumer_render(BRS_GUI_Menu *menu, void *context) {
    BRS_MenuBar_RenderContext *ctx = context;
    ctx->menu = menu;
    BRS_GUI_Menu_render(ctx);
    ctx->menuIndex++;
}

static void BRS_GUI_MenuBar_renderMenus(BRS_MenuBar_RenderContext *renderContext) {
    BRS_GUI_MenuBar *menuBar = BRS_GUI_MenuBar_getFromWidget(renderContext->menuBarWidget);
    BRS_GUI_MenuList_iterate(menuBar->menuList, BRS_MenuConsumer_render, renderContext);
}

void BRS_GUI_MenuBar_render(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_setColor(context, widget->theme->menuBarColor);
    BRS_Rect r = {.x = widget->position->x, .y = widget->position->y, .width=widget->size->width, .height=widget->size->height};
    BRS_drawlFillRect(context, &r);
    BRS_MenuBar_RenderContext renderContext = {.videoContext = context, .menuBarWidget = widget};
    BRS_GUI_MenuBar_renderMenus(&renderContext);
}

static void BRS_GUI_MenuItem_calculate(BRS_MenuBar_RenderContext *renderContext) {
    calculateMenuItemPosition(renderContext);
}

static void BRS_MenuItemConsumer_calculate(BRS_GUI_MenuItem *menuItem, void *context) {
    BRS_MenuBar_RenderContext *ctx = context;
    ctx->menuItem = menuItem;
    BRS_GUI_MenuItem_calculate(ctx);
    ctx->menuItemIndex++;
}

static void
BRS_GUI_Menu_calculate(BRS_MenuBar_RenderContext *renderContext) {
    BRS_GUI_Menu_calcPosition(renderContext);

    if (renderContext->menu->selected) {
        BRS_GUI_MenuItemList_iterate(renderContext->menu->itemList, BRS_MenuItemConsumer_calculate, renderContext);
    }
}

static void BRS_MenuConsumer_calculate(BRS_GUI_Menu *menu, void *context) {
    BRS_MenuBar_RenderContext *ctx = context;
    ctx->menu = menu;
    BRS_GUI_Menu_calculate(ctx);
    ctx->menuIndex++;
}

static void BRS_GUI_MenuBar_calculateMenus(BRS_MenuBar_RenderContext *renderContext) {
    BRS_GUI_MenuBar *menuBar = BRS_GUI_MenuBar_getFromWidget(renderContext->menuBarWidget);
    BRS_GUI_MenuList_iterate(menuBar->menuList, BRS_MenuConsumer_calculate, renderContext);
}

void BRS_GUI_MenuBar_calculate(BRS_VideoContext *context, BRS_GUI_Widget *widget) {
    BRS_MenuBar_RenderContext renderContext = {.videoContext = context, .menuBarWidget = widget, .menu = NULL, .menuItem = NULL};
    BRS_GUI_MenuBar_calculateMenus(&renderContext);
}

static void processMouseButtonDown(BRS_GUI_Widget *widget, SDL_MouseButtonEvent *button) {
    BRS_GUI_MenuBar *menuBar = widget->object;
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        BRS_Rect menuBarRect = {.x = widget->position->x, .y = widget->position->y, .width = widget->size->width, .height = widget->size->height};

        if (BRS_PointInRect(&mousePoint, &menuBarRect)) {
            menuBar->clickHandler(menuBar);
        }
    }
}

static void
processMenuItemMouseButtonDown(BRS_GUI_MenuItem *menuItem, SDL_MouseButtonEvent *button, int32_t menuItemIndex,
                               BRS_GUI_Menu *menu, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    if (button->button == SDL_BUTTON_LEFT) {
        BRS_Rect menuItemRect = {.x = menuItem->position->x, .y = menuItem->position->y, .width = menuItem->size->width,
                menuItem->size->height};
        BRS_Point mousePoint = {.x = button->x, .y = button->y};
        bool clickedInMenuItem = BRS_PointInRect(&mousePoint, &menuItemRect);

        if (clickedInMenuItem && menuItem->clickHandler) {
            menuItem->clickHandler(menuItem);
        }
    }
}

static void processMenuItemMouseMove(BRS_GUI_MenuItem *menuItem, SDL_MouseMotionEvent *motion, int32_t menuItemIndex,
                                     BRS_GUI_Menu *menu, int32_t menuIndex, BRS_GUI_Widget *menuBarWidget) {
    BRS_Point mousePoint = {.x = motion->x, .y = motion->y};
    BRS_Rect widgetRect = {.x = menuItem->position->x, .y = menuItem->position->y, .width = menuItem->size->width, .height = menuItem->size->height};
    menuItem->highlighted = BRS_PointInRect(&mousePoint, &widgetRect);
}

static void
BRS_GUI_MenuItem_processEvent(BRS_MenuBar_ProcessEventContext *context) {
    SDL_Event *event = context->event;
    BRS_GUI_MenuItem *menuItem = context->menuItem;
    switch (event->type) {
        case SDL_MOUSEMOTION:
            processMenuItemMouseMove(menuItem, &event->motion, context->menuItemIndex, context->menu,
                                     context->menuIndex, context->menuBarWidget);
            break;
        case SDL_MOUSEBUTTONUP:
            processMenuItemMouseButtonDown(menuItem, &event->button, context->menuItemIndex, context->menu,
                                           context->menuIndex, context->menuBarWidget);
            break;
    }
}

static void BRS_MenuItemConsumer_processEvent(BRS_GUI_MenuItem *menuItem, void *context) {
    BRS_MenuBar_ProcessEventContext *ctx = context;
    ctx->menuItem = menuItem;
    BRS_GUI_MenuItem_processEvent(ctx);
    ctx->menuItemIndex++;
}

static void
BRS_GUI_Menu_processEvent(BRS_MenuBar_ProcessEventContext *context) {
    SDL_Event *event = context->event;
    BRS_GUI_Menu *menu = context->menu;

    if (event->type == SDL_MOUSEMOTION && menu->selected) {
        BRS_Size *menuItemSize = menu->itemList->firstEntry->value->size;
        int16_t menuItemsHeight = menu->itemList->size * menuItemSize->height;

        BRS_Rect menuRect = {.x = menu->position->x, .y = menu->position->y, .width = menu->size->width, .height =
        menu->size->height + menuItemsHeight};

        BRS_Point mousePoint = {.x = event->motion.x, .y = event->motion.y};
        if (!BRS_PointInRect(&mousePoint, &menuRect)) {
            menu->selected = false;
        }
    }
    if (!menu->selected) {
        return;
    }

    BRS_GUI_MenuItemList_iterate(menu->itemList, BRS_MenuItemConsumer_processEvent, context);
}

static void BRS_MenuConsumer_processEvent(BRS_GUI_Menu *menu, void *context) {
    BRS_MenuBar_ProcessEventContext *ctx = context;
    ctx->menu = menu;
    BRS_GUI_Menu_processEvent(ctx);
    ctx->menuIndex++;
}

bool BRS_GUI_MenuBar_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    BRS_GUI_MenuBar *menuBar = widget->object;
    switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            processMouseButtonDown(widget, &event->button);
            break;
    }

    BRS_MenuBar_ProcessEventContext context = {.event = event, .menuBarWidget = widget};
    BRS_GUI_MenuList_iterate(menuBar->menuList, BRS_MenuConsumer_processEvent, &context);

    return false;
}

BRS_GUI_MenuBar *BRS_GUI_MenuBar_getFromWidget(BRS_GUI_Widget *widget) {
    return widget->object;
}
