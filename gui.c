//
// Created by bernd on 05.01.21.
//
#include "gui.h"

static BRS_GUI_Widget *getCharEditWidget() {
    BRS_GUI_WidgetList *widgetList = getWidgetList();
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->type == BRS_GUI_WIDGET_CHAREDIT) {
            return widget;
        }
        entry = entry->next;
    }
    return NULL;
}

static void onClickMenuBar(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_Menu *firstMenu = menuBar->menuList->firstEntry->value;
    firstMenu->selected = !firstMenu->selected;
}

static void onClickCharTable(BRS_GUI_CharTable *charTable) {
    BRS_GUI_Widget *charEditWidget = getCharEditWidget();
    charEditWidget->object->charEdit->selectedChar = charTable->selectedChar;
}

static void onClickMenuItemQuit(BRS_GUI_MenuItem *menuItem) {
    quitApplication();
}

static BRS_GUI_Menu *createFileMenu(BRS_GUI_MenuBar *menuBar, BRS_Font *font) {
    BRS_Dimension dim = {.width = 50, .height = 20};
    BRS_GUI_Menu *menu = BRS_GUI_Menu_create(menuBar, &dim, "File", &COLOR_WHITE, &COLOR_BLUE, &COLOR_DARK_GRAY, font,
                                             false);
    BRS_GUI_MenuItem *menuItemNew = BRS_GUI_MenuItem_create(&dim, menu, "New", &COLOR_WHITE, &COLOR_BLUE,
                                                            &COLOR_LIGHT_GRAY, font);
    BRS_GUI_MenuItem *menuItemLoad = BRS_GUI_MenuItem_create(&dim, menu, "Load", &COLOR_WHITE, &COLOR_BLUE,
                                                             &COLOR_LIGHT_GRAY, font);
    BRS_GUI_MenuItem *menuItemSave = BRS_GUI_MenuItem_create(&dim, menu, "Save", &COLOR_WHITE, &COLOR_BLUE,
                                                             &COLOR_LIGHT_GRAY, font);
    BRS_GUI_MenuItem *menuItemQuit = BRS_GUI_MenuItem_create(&dim, menu, "Quit", &COLOR_WHITE, &COLOR_BLUE,
                                                             &COLOR_LIGHT_GRAY, font);

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

    BRS_GUI_Menu *fileMenu = createFileMenu(widget->object->menuBar, font);
    BRS_GUI_MenuList_push(fileMenu, widget->object->menuBar->menuList);

    return widget;
}

static BRS_GUI_Widget *createCharTable(BRS_Font *font) {
    BRS_Point position = {.x = 10, .y = 50};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharTable(&position, &COLOR_RED, &COLOR_YELLOW, &COLOR_LIGHT_GRAY,
                                                            &COLOR_DARK_GRAY, font);
    BRS_GUI_CharTable_setClickHandler(widget->object->charTable, onClickCharTable);
    return widget;
}

static BRS_GUI_Widget *createCharEdit(BRS_Font *font) {
    BRS_Point position = {.x = 500, .y = 50};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharEdit(&position, &COLOR_RED, &COLOR_YELLOW, &COLOR_BLACK, font);
    return widget;
}

BRS_GUI_WidgetList *createWidgets(BRS_Font *font, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_WidgetList *list = BRS_GUI_WidgetList_create();

    BRS_Point *labelPosition = malloc(sizeof(BRS_Point));
    const char *labelText = "Ready.";

    labelPosition->x = (screenWidth - strlen(labelText) * font->width_bits) / 2;
    labelPosition->y = screenHeight - font->height_bits - 1;

    BRS_GUI_WidgetList_push(createCharTable(font), list);
    BRS_GUI_WidgetList_push(createCharEdit(font), list);
    BRS_GUI_WidgetList_push(BRS_GUI_Widget_createLabel(labelPosition, &COLOR_LIGHT_GRAY, labelText, font), list);
    BRS_GUI_WidgetList_push(createMenuBar(font), list);

    return list;
}

void destroyWidgets(BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_destroyWidget(entry->value);
        entry = entry->next;
    }
    BRS_GUI_WidgetList_destroy(widgetList);
}