//
// Created by bernd on 05.01.21.
//
#include "gui.h"

BRS_GUI_Widget *BRS_GUI_Widget_getByType(BRS_GUI_WidgetType type) {
    BRS_GUI_WidgetList *widgetList = getWidgetList();
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->type == type) {
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
    BRS_GUI_Widget *charEditWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHAREDIT);
    charEditWidget->object->charEdit->selectedChar = charTable->selectedCharIndex;
}

static void onChangeCharTableSelectedCharIndex(BRS_GUI_CharTable *charTable) {
    BRS_GUI_Widget *charEditWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHAREDIT);
    charEditWidget->object->charEdit->selectedChar = charTable->selectedCharIndex;
}

static void onClickMenuItemLoad(BRS_GUI_MenuItem *load) {
    BRS_GUI_Widget *inputBoxWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_INPUTBOX);
    BRS_GUI_InputBox *inputBox = inputBoxWidget->object->inputBox;
    inputBox->title = "Load file";
    inputBox->textLabel = "Name?";
    BRS_GUI_InputBox_clearText(inputBox);
    inputBox->visible = true;
}

static void onClickMenuItemQuit(BRS_GUI_MenuItem *menuItem) {
    BRS_FontEdit_quitApplication();
}

static BRS_GUI_Menu *createFileMenu(BRS_GUI_MenuBar *menuBar, BRS_GUI_Theme *theme) {
    BRS_Dimension dim = {.width = 50, .height = 20};
    BRS_GUI_Menu *menu = BRS_GUI_Menu_create(menuBar, &dim, "File", theme, false);
    BRS_GUI_MenuItem *menuItemNew = BRS_GUI_MenuItem_create(&dim, menu, "New", theme);
    BRS_GUI_MenuItem *menuItemLoad = BRS_GUI_MenuItem_create(&dim, menu, "Load", theme);
    BRS_GUI_MenuItem *menuItemSave = BRS_GUI_MenuItem_create(&dim, menu, "Save", theme);
    BRS_GUI_MenuItem *menuItemQuit = BRS_GUI_MenuItem_create(&dim, menu, "Quit", theme);

    BRS_GUI_MenuItemList_push(menuItemNew, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemLoad, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemSave, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemQuit, menu->itemList);

    BRS_GUI_setMenuItemClickHandler(menuItemLoad, onClickMenuItemLoad);
    BRS_GUI_setMenuItemClickHandler(menuItemQuit, onClickMenuItemQuit);

    return menu;
}

static BRS_GUI_Widget *createMenuBar(BRS_GUI_Theme *theme) {
    BRS_Point *menuBarPosition = malloc(sizeof(BRS_Point));
    menuBarPosition->x = 0;
    menuBarPosition->y = 0;
    BRS_Dimension *menuBarDimension = malloc(sizeof(struct BRS_Dimension));
    menuBarDimension->width = 800;
    menuBarDimension->height = 20;
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createMenuBar(menuBarPosition, menuBarDimension, theme);

    BRS_GUI_Widget_setClickHandler(widget, &onClickMenuBar);

    BRS_GUI_Menu *fileMenu = createFileMenu(widget->object->menuBar, theme);
    BRS_GUI_MenuList_push(fileMenu, widget->object->menuBar->menuList);

    return widget;
}

static BRS_GUI_Widget *createCharTable(BRS_Font *fontEdited, BRS_GUI_Theme *theme) {
    BRS_Point position = {.x = 10, .y = 50};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharTable(&position, theme, fontEdited);
    BRS_GUI_CharTable_setClickHandler(widget->object->charTable, onClickCharTable);
    BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(widget->object->charTable, onChangeCharTableSelectedCharIndex);
    return widget;
}

static BRS_GUI_Widget *createCharEdit(BRS_Font *fontEdited, BRS_GUI_Theme *theme) {
    BRS_Point position = {.x = 500, .y = 50};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharEdit(&position, theme, fontEdited);
    return widget;
}

static BRS_GUI_Widget *createInputBox(BRS_GUI_Theme *theme, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_Dimension dimension = {.width = 200, .height = 100};
    BRS_Point position = {.x = (screenWidth - dimension.width) / 2, .y = (screenHeight - dimension.height) / 2 -
                                                                         dimension.height};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createInputBox(&position, &dimension, theme, "Input", "?");
    return widget;
}

BRS_GUI_WidgetList *
createWidgets(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_WidgetList *list = BRS_GUI_WidgetList_create();

    BRS_Point *labelPosition = malloc(sizeof(BRS_Point));
    const char *labelText = "Ready.";

    labelPosition->x = (screenWidth - strlen(labelText) * theme->font->width_bits) / 2;
    labelPosition->y = screenHeight - theme->font->height_bits - 1;

    BRS_GUI_WidgetList_push(createCharTable(fontEdited, theme), list);
    BRS_GUI_WidgetList_push(createCharEdit(fontEdited, theme), list);
    BRS_GUI_WidgetList_push(BRS_GUI_Widget_createLabel(labelPosition, theme, labelText), list);
    BRS_GUI_WidgetList_push(createMenuBar(theme), list);
    BRS_GUI_WidgetList_push(createInputBox(theme, screenWidth, screenHeight), list);

    return list;
}

void destroyWidgets(BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget_destroy(entry->value);
        entry = entry->next;
    }
    BRS_GUI_WidgetList_destroy(widgetList);
}