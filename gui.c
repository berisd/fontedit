//
// Created by bernd on 05.01.21.
//
#include "gui.h"

static void onClickMenuBar(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_Menu *firstMenu = menuBar->menuList->firstEntry->value;
    firstMenu->selected = !firstMenu->selected;
}

static void onClickMenuItemQuit(BRS_GUI_MenuItem *menuItem) {
    quitApplication();
}

static BRS_GUI_Menu *createFileMenu(BRS_Font *font) {
    BRS_GUI_Menu *menu = BRS_GUI_Menu_create("File", &COLOR_WHITE, &COLOR_BLUE, &COLOR_YELLOW, font, false);
    BRS_GUI_MenuItem *menuItemNew = BRS_GUI_MenuItem_create("New", &COLOR_WHITE, &COLOR_BLUE, font);
    BRS_GUI_MenuItem *menuItemLoad = BRS_GUI_MenuItem_create("Load", &COLOR_WHITE, &COLOR_BLUE, font);
    BRS_GUI_MenuItem *menuItemSave = BRS_GUI_MenuItem_create("Save", &COLOR_WHITE, &COLOR_BLUE, font);
    BRS_GUI_MenuItem *menuItemQuit = BRS_GUI_MenuItem_create("Quit", &COLOR_WHITE, &COLOR_BLUE, font);

    BRS_GUI_MenuItemList_push(menuItemNew, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemLoad, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemSave, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemQuit, menu->itemList);

    BRS_GUI_setMenuItemClickHandler(menuItemQuit, onClickMenuItemQuit);

    return menu;
}

static BRS_GUI_Widget *createMenuBar(BRS_Font *font) {
    BRS_Point *menuBarPosition = malloc(sizeof(BRS_Point));
    menuBarPosition->x = 0;
    menuBarPosition->y = 0;
    BRS_Dimension *menuBarDimension = malloc(sizeof(struct BRS_Dimension));
    menuBarDimension->width = 800;
    menuBarDimension->height = 20;
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createMenuBar(menuBarPosition, menuBarDimension, &COLOR_BLUE, font);

    BRS_GUI_setMenuBarClickHandler(widget, &onClickMenuBar);

    BRS_GUI_Menu *fileMenu = createFileMenu(font);
    BRS_GUI_MenuList_push(fileMenu, widget->object->menuBar->menuList);

    return widget;
}

BRS_GUI_WidgetList *createWidgets(BRS_Font *font) {
    BRS_GUI_WidgetList *list = BRS_GUI_WidgetList_create();

    BRS_Point *labelPosition = malloc(sizeof(BRS_Point));
    labelPosition->x = 100;
    labelPosition->y = 150;

    BRS_GUI_WidgetList_push(BRS_GUI_Widget_createLabel(labelPosition, &COLOR_RED, "Hello World", font), list);
    BRS_GUI_WidgetList_push(createMenuBar(font), list);

    return list;
}

void destroyWidgets(BRS_GUI_WidgetList *widgets) {
    BRS_GUI_WidgetListEntry *entry = widgets->firstEntry;
    while (entry != NULL) {
        BRS_GUI_destroyWidget(entry->value);
        BRS_GUI_WidgetListEntry *tmp = entry;
        entry = entry->next;
        free(tmp);
    }
    free(widgets);
}