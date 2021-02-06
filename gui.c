//
// Created by bernd on 05.01.21.
//
#include "gui.h"
#include "widget/widgets.h"

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
    inputBoxWidget->properties->visible = true;
}

static void onClickMenuItemSave(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_Widget *inputBoxWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_INPUTBOX, BRS_FontEdit_getWidgetList());
    BRS_GUI_InputBox *inputBox = inputBoxWidget->object;
    BRS_GUI_InputBox_setConfirmHandler(inputBox, onConfirmInputBoxSave);
    inputBox->title = "Save file";
    inputBox->textLabel = "Name?";
    BRS_GUI_InputBox_clearText(inputBox);
    inputBoxWidget->properties->visible = true;
}

static void onClickMenuItemNew(BRS_GUI_MenuItem *menuItem) {
    BRS_GUI_Label *label = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_LABEL, BRS_FontEdit_getWidgetList())->object;
    BRS_FontEdit_createFont();
    BRS_GUI_Label_setText(label, "New font created!");
    BRS_GUI_Widget *charTableWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHARTABLE, BRS_FontEdit_getWidgetList());
    BRS_GUI_CharTable_setSelectedCharIndex(charTableWidget->object, 0);
}

static void onClickMenuItemQuit(BRS_GUI_MenuItem *menuItem) {
    BRS_FontEdit_quitApplication();
}

static BRS_GUI_Menu *createFileMenu() {
    BRS_Size menuItemSize = {.width = 100, .height = 20};
    BRS_Padding padding = {.left = 5, .right = 0, .top = 0, .bottom = 0};
    BRS_GUI_Menu *menu = BRS_GUI_Menu_create("File");
    BRS_GUI_MenuItem *menuItemNew = BRS_GUI_MenuItem_create(&menuItemSize, &padding, "New");
    BRS_GUI_MenuItem *menuItemLoad = BRS_GUI_MenuItem_create(&menuItemSize, &padding, "Load");
    BRS_GUI_MenuItem *menuItemSave = BRS_GUI_MenuItem_create(&menuItemSize, &padding, "Save");
    BRS_GUI_MenuItem *menuItemQuit = BRS_GUI_MenuItem_create(&menuItemSize, &padding, "Quit");

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
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create();
    widgetProps->size->width = 800;
    widgetProps->size->height = 20;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    BRS_GUI_Widget *menuBarWidget = BRS_GUI_Widget_createMenuBar(widgetProps);

    BRS_GUI_Widget_Properties *widgetFileMenuProps = BRS_GUI_Widget_Properties_create();
    widgetFileMenuProps->size->width = 100;
    widgetFileMenuProps->size->height = 20;
    widgetFileMenuProps->padding->left = 5;
    widgetFileMenuProps->theme = theme;

    BRS_GUI_Menu *fileMenu = createFileMenu();
    BRS_GUI_WidgetList_push(BRS_GUI_Widget_createMenu(widgetFileMenuProps, fileMenu, menuBarWidget),
                            menuBarWidget->children);

    return menuBarWidget;
}

static BRS_GUI_Widget *createLabelStatus(BRS_GUI_Theme *theme, BRS_Size *windowSize) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create();
    widgetProps->position->x = 1;
    widgetProps->position->y = windowSize->height - theme->font->height_bits - 1;
    widgetProps->theme = theme;
    widgetProps->visible = true;

    const char *labelText = "Ready.";
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createLabel(widgetProps, labelText);
    return widget;
}

static BRS_GUI_Widget *createCharTable(BRS_GUI_Theme *theme, BRS_Font *fontEdited) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create();
    widgetProps->position->x = 10;
    widgetProps->position->y = 60;
    widgetProps->theme = theme;
    widgetProps->visible = true;

    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharTable(widgetProps, fontEdited);
    BRS_GUI_Widget_setClickHandler(widget, onClickCharTable);
    BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(BRS_GUI_CharTable_getFromWidget(widget),
                                                         onChangeCharTableSelectedCharIndex);
    return widget;
}

static BRS_GUI_Widget *createCharEdit(BRS_GUI_Theme *theme, BRS_Font *fontEdited) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create();
    widgetProps->position->x = 500;
    widgetProps->position->y = 50;
    widgetProps->theme = theme;
    widgetProps->visible = true;

    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharEdit(widgetProps, fontEdited);

    return widget;
}

static BRS_GUI_Widget *createInputBox(BRS_GUI_Theme *theme, BRS_Size *windowSize) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create();
    widgetProps->size->width = 200;
    widgetProps->size->height = 100;
    widgetProps->position->x = (windowSize->width - widgetProps->size->width) / 2;
    widgetProps->position->y = (windowSize->height - widgetProps->size->height) / 2 - widgetProps->size->height;
    widgetProps->theme = theme;
    widgetProps->zIndex = 100;

    BRS_GUI_Widget *widget = BRS_GUI_Widget_createInputBox(widgetProps, "Input", "?");
    BRS_GUI_InputBox_setCancelHandler(widget->object, onCancelInputBox);
    return widget;
}

static BRS_GUI_Widget *
createMainWindow(BRS_GUI_Theme *theme, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create();
    widgetProps->size->width = screenWidth;
    widgetProps->size->height = screenHeight;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    BRS_GUI_Widget *window = BRS_GUI_Widget_createWindow(widgetProps);
    return window;
}

BRS_GUI *
BRS_GUI_create(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_WidgetList *widgetList = BRS_GUI_WidgetList_create();
    BRS_GUI_Widget *window = createMainWindow(theme, screenWidth, screenHeight);
    BRS_GUI_WidgetList_push(window, widgetList);

    BRS_GUI_WidgetList_push(createCharTable(theme, fontEdited), window->children);
    BRS_GUI_WidgetList_push(createCharEdit(theme, fontEdited), window->children);
    BRS_GUI_WidgetList_push(createLabelStatus(theme, window->properties->size), window->children);
    BRS_GUI_WidgetList_push(createMenuBar(theme), window->children);
    BRS_GUI_WidgetList_push(createInputBox(theme, window->properties->size), window->children);

    BRS_GUI *gui = malloc(sizeof(BRS_GUI));
    gui->widgetList = widgetList;

    return gui;
}

void BRS_GUI_init(BRS_GUI *gui) {
    BRS_GUI_CharTable_setSelectedCharIndex(
            BRS_GUI_CharTable_getFromWidget(BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHARTABLE, gui->widgetList)), 0);
}

void BRS_GUI_destroy(BRS_GUI *gui) {
    BRS_GUI_WidgetList *widgetList = gui->widgetList;
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget_destroy(entry->value);
        entry = entry->next;
    }
    BRS_GUI_WidgetList_destroy(widgetList);
    free(gui);
}

static void BRS_GUI_renderWidgetList(BRS_VideoContext *videoContext, BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *listEntry = widgetList->firstEntry;
    while (listEntry != NULL) {
        BRS_GUI_Widget *widget = listEntry->value;
        if (widget->properties->visible) {
            BRS_GUI_Widget_render(videoContext, widget);
        }
        BRS_GUI_renderWidgetList(videoContext, widget->children);
        listEntry = listEntry->next;
    }
}

static void BRS_GUI_calculateWidgetList(BRS_GUI_WidgetList *widgetList, BRS_GUI_Widget *parentWidget) {
    BRS_GUI_WidgetListEntry *listEntry = widgetList->firstEntry;
    int32_t widgetIndex = 0;
    while (listEntry != NULL) {
        BRS_GUI_Widget *widget = listEntry->value;
        BRS_GUI_Widget_calculate(widget, parentWidget, widgetIndex);
        BRS_GUI_calculateWidgetList(widget->children, widget);
        listEntry = listEntry->next;
        widgetIndex++;
    }
}

void BRS_GUI_calculate(BRS_GUI *gui) {
    BRS_GUI_calculateWidgetList(gui->widgetList, NULL);
}

void BRS_GUI_render(BRS_VideoContext *videoContext, BRS_GUI *gui) {
    BRS_GUI_renderWidgetList(videoContext, gui->widgetList);
}

static bool
BRS_GUI_processEventForList(BRS_GUI_WidgetList *widgetList, SDL_Event *event, int16_t *zIndexHighestVisible,
                            uint8_t depth) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->properties->zIndex == zIndexHighestVisible[depth]) {
            if (BRS_GUI_Widget_processEvent(widget, event)) {
                return true;
            }
            depth++;
            if (BRS_GUI_processEventForList(widget->children, event, zIndexHighestVisible, depth)) {
                return true;
            }
            depth--;
        }
        entry = entry->next;
    }
    return false;
}

static int16_t calcZIndexHighestVisible(BRS_GUI_WidgetList *widgetList, int16_t *zIndexHighestVisible, uint8_t depth) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->properties->visible && widget->properties->zIndex > zIndexHighestVisible[depth]) {
            zIndexHighestVisible[depth] = widget->properties->zIndex;
        }
        depth++;
        calcZIndexHighestVisible(widget->children, zIndexHighestVisible, depth);
        depth--;
        entry = entry->next;
    }
}

bool BRS_GUI_processEvent(BRS_GUI *gui, SDL_Event *event) {
    int16_t zIndexHighestVisible[BRS_GUI_WIDGET_DEPTH_MAX] = {0};
    calcZIndexHighestVisible(gui->widgetList, zIndexHighestVisible, 0);
    return BRS_GUI_processEventForList(gui->widgetList, event, zIndexHighestVisible, 0);
}
