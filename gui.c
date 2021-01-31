//
// Created by bernd on 05.01.21.
//
#include "gui.h"
#include "widget/widgets.h"

static void onClickMenuBar(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_Menu *firstMenu = menuBar->menuList->firstEntry->value;
    firstMenu->selected = !firstMenu->selected;
}

static void onClickCharTable(BRS_GUI_CharTable *charTable) {
    BRS_GUI_Widget *charEditWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHAREDIT, BRS_FontEdit_getWidgetList());
    ((BRS_GUI_CharEdit *) charEditWidget->object)->selectedChar = charTable->selectedCharIndex;
}

static void onConfirmInputBoxLoad(BRS_GUI_InputBox *inputBox) {
    BRS_FontEdit_loadFont(inputBox->text);
}

static void onConfirmInputBoxSave(BRS_GUI_InputBox *inputBox) {
    BRS_FontEdit_saveFont(inputBox->text);
}

static void onCancelInputBox(BRS_GUI_InputBox *inputBox) {
}

static void onChangeCharTableSelectedCharIndex(BRS_GUI_CharTable *charTable) {
    BRS_GUI_Widget *charEditWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHAREDIT, BRS_FontEdit_getWidgetList());
    ((BRS_GUI_CharEdit *) charEditWidget->object)->selectedChar = charTable->selectedCharIndex;
}

static void onClickMenuItemLoad(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_Widget *inputBoxWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_INPUTBOX, BRS_FontEdit_getWidgetList());
    BRS_GUI_InputBox *inputBox = inputBoxWidget->object;
    BRS_GUI_InputBox_setConfirmHandler(inputBox, onConfirmInputBoxLoad);
    inputBox->title = "Load file";
    inputBox->textLabel = "Name?";
    BRS_GUI_InputBox_clearText(inputBox);
    inputBox->visible = true;
}

static void onClickMenuItemSave(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_Widget *inputBoxWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_INPUTBOX, BRS_FontEdit_getWidgetList());
    BRS_GUI_InputBox *inputBox = inputBoxWidget->object;
    BRS_GUI_InputBox_setConfirmHandler(inputBox, onConfirmInputBoxSave);
    inputBox->title = "Save file";
    inputBox->textLabel = "Name?";
    BRS_GUI_InputBox_clearText(inputBox);
    inputBox->visible = true;
}

static void onClickMenuItemNew(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_Label *label = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_LABEL, BRS_FontEdit_getWidgetList())->object;
    BRS_FontEdit_createFont();
    BRS_GUI_Label_setText(label, "New font created!");
}

static void onClickMenuItemQuit(BRS_GUI_MenuItem *menuItem) {
    BRS_FontEdit_quitApplication();
}

static BRS_GUI_Menu *createFileMenu() {
    BRS_Size dim = {.width = 50, .height = 20};
    BRS_GUI_Menu *menu = BRS_GUI_Menu_create(&dim, "File", false);
    BRS_GUI_MenuItem *menuItemNew = BRS_GUI_MenuItem_create(&dim, "New");
    BRS_GUI_MenuItem *menuItemLoad = BRS_GUI_MenuItem_create(&dim, "Load");
    BRS_GUI_MenuItem *menuItemSave = BRS_GUI_MenuItem_create(&dim, "Save");
    BRS_GUI_MenuItem *menuItemQuit = BRS_GUI_MenuItem_create(&dim, "Quit");

    BRS_GUI_MenuItemList_push(menuItemNew, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemLoad, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemSave, menu->itemList);
    BRS_GUI_MenuItemList_push(menuItemQuit, menu->itemList);

    BRS_GUI_setMenuItemClickHandler(menuItemNew, onClickMenuItemNew);
    BRS_GUI_setMenuItemClickHandler(menuItemLoad, onClickMenuItemLoad);
    BRS_GUI_setMenuItemClickHandler(menuItemSave, onClickMenuItemSave);
    BRS_GUI_setMenuItemClickHandler(menuItemQuit, onClickMenuItemQuit);

    return menu;
}

static BRS_GUI_Widget *createMenuBar(BRS_GUI_Theme *theme) {
    BRS_Point *menuBarPosition = malloc(sizeof(BRS_Point));
    menuBarPosition->x = 0;
    menuBarPosition->y = 0;
    BRS_Size *menuBarSize = malloc(sizeof(BRS_Size));
    menuBarSize->width = 800;
    menuBarSize->height = 20;
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createMenuBar(menuBarPosition, menuBarSize, theme);

    BRS_GUI_Widget_setClickHandler(widget, &onClickMenuBar);

    BRS_GUI_Menu *fileMenu = createFileMenu();
    BRS_GUI_MenuList_push(fileMenu, BRS_GUI_MenuBar_getFromWidget(widget)->menuList);

    return widget;
}

static BRS_GUI_Widget *createCharTable(BRS_Font *fontEdited, BRS_GUI_Theme *theme) {
    BRS_Point position = {.x = 10, .y = 50};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharTable(&position, theme, fontEdited);
    BRS_GUI_Widget_setClickHandler(widget, onClickCharTable);
    BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(BRS_GUI_CharTable_getFromWidget(widget),
                                                         onChangeCharTableSelectedCharIndex);
    return widget;
}

static BRS_GUI_Widget *createCharEdit(BRS_Font *fontEdited, BRS_GUI_Theme *theme) {
    BRS_Point position = {.x = 500, .y = 50};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharEdit(&position, theme, fontEdited);
    return widget;
}

static BRS_GUI_Widget *createInputBox(BRS_GUI_Theme *theme, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_Size size = {.width = 200, .height = 100};
    BRS_Point position = {.x = (screenWidth - size.width) / 2, .y = (screenHeight - size.height) / 2 -
                                                                    size.height};
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createInputBox(&position, &size, theme, "Input", "?");
    BRS_GUI_InputBox_setCancelHandler(widget->object, onCancelInputBox);
    return widget;
}

static BRS_GUI_Widget *
createMainWindow(BRS_GUI_Theme *theme, BRS_Font *fontEdited, BRS_Point *windowOrigin, BRS_Size *windowSize) {
    BRS_GUI_Widget *window = BRS_GUI_Widget_createWindow(windowOrigin, windowSize, theme);

    BRS_Point *labelPosition = malloc(sizeof(BRS_Point));
    const char *labelText = "Ready.";

    labelPosition->x = 1;
    labelPosition->y = windowSize->height - theme->font->height_bits - 1;

    BRS_GUI_WidgetList_push(createCharTable(fontEdited, theme), window->children);
    BRS_GUI_WidgetList_push(createCharEdit(fontEdited, theme), window->children);
    BRS_GUI_WidgetList_push(BRS_GUI_Widget_createLabel(labelPosition, theme, labelText), window->children);
    BRS_GUI_WidgetList_push(createMenuBar(theme), window->children);
    BRS_GUI_WidgetList_push(createInputBox(theme, windowSize->width, windowSize->height), window->children);

    return window;
}

BRS_GUI *
BRS_GUI_createGUI(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_Point windowOrigin = {.x = 0, .y=0};
    BRS_Size windowSize = {.width = screenWidth, .height = screenHeight};
    BRS_GUI_Widget *window = createMainWindow(theme, fontEdited, &windowOrigin, &windowSize);
    BRS_GUI_WidgetList *list = BRS_GUI_WidgetList_create();
    BRS_GUI_WidgetList_push(window, list);
    return list;
}

void BRS_GUI_initGUI(BRS_GUI_WidgetList *gui) {
    BRS_GUI_CharTable_setSelectedCharIndex(
            BRS_GUI_CharTable_getFromWidget(BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHARTABLE, gui)), 0);
}

void BRS_GUI_destroyGUI(BRS_GUI_WidgetList *gui) {
    BRS_GUI_WidgetListEntry *entry = gui->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget_destroy(entry->value);
        entry = entry->next;
    }
    BRS_GUI_WidgetList_destroy(gui);
}

static void BRS_GUI_renderWidgetList(BRS_VideoContext *videoContext, BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *listEntry = widgetList->firstEntry;
    while (listEntry != NULL) {
        BRS_GUI_Widget *widget = listEntry->value;
        BRS_GUI_Widget_render(videoContext, widget);
        BRS_GUI_renderWidgetList(videoContext, widget->children);
        listEntry = listEntry->next;
    }
}

void BRS_GUI_renderGUI(BRS_VideoContext *videoContext, BRS_GUI_WidgetList *gui) {
    BRS_GUI_renderWidgetList(videoContext, gui);
}

static bool BRS_GUI_processEventForList(BRS_GUI_WidgetList *widgetList, SDL_Event *event) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (BRS_GUI_Widget_processEvent(widget, event)) {
            return true;
        }
        if (BRS_GUI_processEventForList(widget->children, event)) {
            return true;
        }
        entry = entry->next;
    }
    return false;
}

bool BRS_GUI_processEvent(BRS_GUI *gui, SDL_Event *event) {
    return BRS_GUI_processEventForList(gui, event);
}
