//
// Created by bernd on 05.01.21.
//
#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "widget/char_table.h"
#include "widget/char_edit.h"
#include "widget/label.h"
#include "widget/inputbox.h"
#include "widget/window.h"
#include "widget/menu_bar.h"
#include "widget/menu.h"
#include "widget/menu_item.h"

static const char *WIDGET_ID_MENU_FILE = "MENU_FILE";
static const char *WIDGET_ID_MENU_BAR = "MENU_BAR";
static const char *WIDGET_ID_LABEL_STATUS = "LABEL_STATUS";
static const char *WIDGET_ID_CHAR_TABLE = "CHAR_TABLE";
static const char *WIDGET_ID_CHAR_EDIT = "CHAR_EDIT";
static const char *WIDGET_ID_INPUT_BOX = "INPUT_BOX";
static const char *WIDGET_ID_WINDOW_MAIN = "WINDOW_MAIN";
static const char *WIDGET_ID_MENU_ITEM_NEW = "MENU_ITEM_NEW";
static const char *WIDGET_ID_MENU_ITEM_LOAD = "MENU_ITEM_LOAD";
static const char *WIDGET_ID_MENU_ITEM_SAVE = "MENU_ITEM_SAVE";
static const char *WIDGET_ID_MENU_ITEM_QUIT = "MENU_ITEM_QUIT";

static void onClickCharTable(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_CharEdit *charEditWidget = (BRS_GUI_CharEdit *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_CHAR_EDIT,
                                                                                          rootWidget);
    BRS_GUI_CharTable *charTable = (BRS_GUI_CharTable *) widget;
    charEditWidget->selectedChar = charTable->selectedCharIndex;
}

static void onConfirmInputBoxLoad(BRS_GUI_InputBox *inputBox) {
    bool success = BRS_FontEdit_loadFont(inputBox->text);
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget((BRS_GUI_Widget *) inputBox);
    BRS_GUI_Label *label = (BRS_GUI_Label *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_LABEL_STATUS, rootWidget);
    if (success) {
        char str[255];
        sprintf(str, "Font %s loaded successfully!", inputBox->text);
        BRS_GUI_Label_setText(label, str);
    } else {
        BRS_GUI_Label_setText(label, "Error loading font!");
    }
}

static void onConfirmInputBoxSave(BRS_GUI_InputBox *inputBox) {
    bool success = BRS_FontEdit_saveFont(inputBox->text);
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget((BRS_GUI_Widget *) inputBox);
    BRS_GUI_Label *label = (BRS_GUI_Label *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_LABEL_STATUS, rootWidget);
    if (success) {
        char str[255];
        sprintf(str, "Font %s saved successfully!", inputBox->text);
        BRS_GUI_Label_setText(label, str);
    } else {
        BRS_GUI_Label_setText(label, "Error saving font!");
    }
}

static void onChangeCharTableSelectedCharIndex(BRS_GUI_CharTable *charTable) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget((BRS_GUI_Widget *) charTable);
    BRS_GUI_CharEdit *charEditWidget = (BRS_GUI_CharEdit *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_CHAR_EDIT,
                                                                                          rootWidget);
    charEditWidget->selectedChar = charTable->selectedCharIndex;
}

static void onClickMenuItemLoad(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_InputBox *inputBox = (BRS_GUI_InputBox *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_INPUT_BOX, rootWidget);
    BRS_GUI_InputBox_setConfirmHandler(inputBox, onConfirmInputBoxLoad);
    BRS_GUI_InputBox_setTitle(inputBox, "Load file");
    BRS_GUI_InputBox_setTextLabel(inputBox, "Name?");
    BRS_GUI_InputBox_clearText(inputBox);
    BRS_GUI_Widget_setVisible((BRS_GUI_Widget *) inputBox, true);
}

static void onClickMenuItemSave(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_InputBox *inputBox = (BRS_GUI_InputBox *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_INPUT_BOX, rootWidget);
    BRS_GUI_InputBox_setConfirmHandler(inputBox, onConfirmInputBoxSave);
    BRS_GUI_InputBox_setTitle(inputBox, "Save file");
    BRS_GUI_InputBox_setTextLabel(inputBox, "Name?");
    BRS_GUI_InputBox_clearText(inputBox);
    BRS_GUI_Widget_setVisible((BRS_GUI_Widget *) inputBox, true);
}

static void onClickMenuItemNew(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Label *label = (BRS_GUI_Label *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_LABEL_STATUS, rootWidget);
    BRS_FontEdit_createFont();
    BRS_GUI_Label_setText(label, "New font created!");
    BRS_GUI_CharTable *charTableWidget = (BRS_GUI_CharTable *) BRS_GUI_Widget_findWidgetById(WIDGET_ID_CHAR_TABLE,
                                                                                             rootWidget);
    BRS_GUI_CharTable_setSelectedCharIndex(charTableWidget, 0);
}

static void onClickMenuItemQuit(BRS_GUI_Widget *widget) {
    BRS_FontEdit_quitApplication();
}

static BRS_GUI_Menu *createFileMenu(BRS_GUI_MenuBar *menuBar) {
    BRS_GUI_Widget_Properties *widgetFileMenuProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_MENU_FILE);
    widgetFileMenuProps->size->width = 100;
    widgetFileMenuProps->size->height = 20;
    widgetFileMenuProps->padding->left = 5;
    widgetFileMenuProps->theme = menuBar->widget.properties->theme;
    widgetFileMenuProps->parent = (BRS_GUI_Widget *) menuBar;

    BRS_GUI_Menu *fileMenu = BRS_GUI_Menu_create(widgetFileMenuProps, "File");

    BRS_GUI_Widget_Properties *menuItemNewWidgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_MENU_ITEM_NEW);
    menuItemNewWidgetProps->size->width = 100;
    menuItemNewWidgetProps->size->height = 20;
    menuItemNewWidgetProps->padding->left = 5;
    menuItemNewWidgetProps->theme = menuBar->widget.properties->theme;
    menuItemNewWidgetProps->parent = (BRS_GUI_Widget *) fileMenu;
    BRS_GUI_MenuItem *menuItemNew = BRS_GUI_MenuItem_create(menuItemNewWidgetProps, "New");
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) menuItemNew, widgetFileMenuProps->children);

    BRS_GUI_Widget_Properties *menuItemLoadWidgetProps = BRS_GUI_Widget_Properties_copy(menuItemNewWidgetProps);
    menuItemLoadWidgetProps->id = strdup(WIDGET_ID_MENU_ITEM_LOAD);
    BRS_GUI_MenuItem *menuItemLoad = BRS_GUI_MenuItem_create(menuItemLoadWidgetProps, "Load");
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) menuItemLoad, widgetFileMenuProps->children);

    BRS_GUI_Widget_Properties *menuItemSaveWidgetProps = BRS_GUI_Widget_Properties_copy(menuItemNewWidgetProps);
    menuItemSaveWidgetProps->id = strdup(WIDGET_ID_MENU_ITEM_SAVE);
    BRS_GUI_MenuItem *menuItemSave = BRS_GUI_MenuItem_create(menuItemSaveWidgetProps, "Save");
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) menuItemSave, widgetFileMenuProps->children);

    BRS_GUI_Widget_Properties *menuItemQuitWidgetProps = BRS_GUI_Widget_Properties_copy(menuItemNewWidgetProps);
    menuItemQuitWidgetProps->id = strdup(WIDGET_ID_MENU_ITEM_QUIT);
    BRS_GUI_MenuItem *menuItemQuit = BRS_GUI_MenuItem_create(menuItemQuitWidgetProps, "Quit");
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) menuItemQuit, widgetFileMenuProps->children);

    menuItemNew->widget.properties->clickHandler = onClickMenuItemNew;
    menuItemLoad->widget.properties->clickHandler = onClickMenuItemLoad;
    menuItemSave->widget.properties->clickHandler = onClickMenuItemSave;
    menuItemQuit->widget.properties->clickHandler = onClickMenuItemQuit;

    return fileMenu;
}

static BRS_GUI_MenuBar *createMenuBar(BRS_GUI_Theme *theme, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_MENU_BAR);
    widgetProps->size->width = 800;
    widgetProps->size->height = 20;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    widgetProps->parent = parent;
    BRS_GUI_MenuBar *menuBar = BRS_GUI_MenuBar_create(widgetProps);

    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) createFileMenu(menuBar), widgetProps->children);

    return menuBar;
}

static BRS_GUI_Label *createLabelStatus(BRS_GUI_Theme *theme, BRS_Size *windowSize, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_LABEL_STATUS);
    widgetProps->position->x = 1;
    widgetProps->position->y = windowSize->height - theme->font->height_bits - 1;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    widgetProps->parent = parent;

    BRS_GUI_Label *label = BRS_GUI_Label_create(widgetProps, "Ready.");
    return label;
}

static BRS_GUI_CharTable *createCharTable(BRS_GUI_Theme *theme, BRS_Font *fontEdited, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_CHAR_TABLE);
    widgetProps->position->x = 10;
    widgetProps->position->y = 60;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    widgetProps->parent = parent;

    BRS_GUI_CharTable *charTable = BRS_GUI_CharTable_create(widgetProps, fontEdited);
    charTable->widget.properties->clickHandler = onClickCharTable;
    BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(charTable, onChangeCharTableSelectedCharIndex);
    return charTable;
}

static BRS_GUI_CharEdit *createCharEdit(BRS_GUI_Theme *theme, BRS_Font *fontEdited, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_CHAR_EDIT);
    widgetProps->position->x = 500;
    widgetProps->position->y = 50;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    widgetProps->parent = parent;

    BRS_GUI_CharEdit *charEdit = BRS_GUI_CharEdit_create(widgetProps, fontEdited);
    return charEdit;
}

static BRS_GUI_InputBox *createInputBox(BRS_GUI_Theme *theme, BRS_Size *windowSize, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_INPUT_BOX);
    widgetProps->size->width = 200;
    widgetProps->size->height = 100;
    widgetProps->position->x = (windowSize->width - widgetProps->size->width) / 2;
    widgetProps->position->y = (windowSize->height - widgetProps->size->height) / 2 - widgetProps->size->height;
    widgetProps->theme = theme;
    widgetProps->zIndex = 100;
    widgetProps->parent = parent;

    BRS_GUI_InputBox *inputBox = BRS_GUI_InputBox_create(widgetProps, "Input", "?");
    return inputBox;
}

static BRS_GUI_Window *
createMainWindow(BRS_GUI_Theme *theme, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_WINDOW_MAIN);
    widgetProps->size->width = screenWidth;
    widgetProps->size->height = screenHeight;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    return BRS_GUI_Window_create(widgetProps);
}

BRS_GUI *
BRS_GUI_create(BRS_GUI_Theme *theme, BRS_Font *fontEdited, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_WidgetList *widgetList = BRS_GUI_WidgetList_create();
    BRS_GUI_Window *window = createMainWindow(theme, screenWidth, screenHeight);
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) window, widgetList);

    BRS_GUI_Widget_Properties *windowWidgetProps = window->widget.properties;
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) createCharTable(theme, fontEdited, (BRS_GUI_Widget *) window),
                            windowWidgetProps->children);
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) createCharEdit(theme, fontEdited, (BRS_GUI_Widget *) window),
                            windowWidgetProps->children);
    BRS_GUI_WidgetList_push(
            (BRS_GUI_Widget *) createLabelStatus(theme, windowWidgetProps->size, (BRS_GUI_Widget *) window),
            windowWidgetProps->children);
    BRS_GUI_WidgetList_push((BRS_GUI_Widget *) createMenuBar(theme, (BRS_GUI_Widget *) window),
                            windowWidgetProps->children);
    BRS_GUI_WidgetList_push(
            (BRS_GUI_Widget *) createInputBox(theme, windowWidgetProps->size, (BRS_GUI_Widget *) window),
            windowWidgetProps->children);

    BRS_GUI *gui = malloc(sizeof(BRS_GUI));
    gui->widgetList = widgetList;

    return gui;
}

void BRS_GUI_init(BRS_GUI *gui) {
    BRS_GUI_CharTable *charTable = (BRS_GUI_CharTable *) BRS_GUI_Widget_getByIdFromList(WIDGET_ID_CHAR_TABLE,
                                                                                        gui->widgetList);
    BRS_GUI_CharTable_setSelectedCharIndex(charTable, 0);
}

void BRS_GUI_destroy(BRS_GUI *gui) {
    BRS_GUI_WidgetList *widgetList = gui->widgetList;
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        BRS_GUI_Widget_destroy(widget);
        entry = entry->next;
    }
    BRS_GUI_WidgetList_destroy(widgetList);
    free(gui);
}

static void BRS_GUI_renderWidgetList(BRS_VideoContext *videoContext, BRS_GUI_WidgetList *widgetList) {
    BRS_GUI_WidgetListEntry *listEntry = widgetList->firstEntry;
    while (listEntry != NULL) {
        BRS_GUI_Widget *widget = listEntry->value;
        BRS_GUI_Widget_render(widget, videoContext);
        BRS_GUI_renderWidgetList(videoContext, widget->properties->children);
        listEntry = listEntry->next;
    }
}

static void BRS_GUI_calculateWidgetList(BRS_GUI_WidgetList *widgetList, BRS_GUI_Widget *parentWidget) {
    BRS_GUI_WidgetListEntry *listEntry = widgetList->firstEntry;
    int32_t widgetIndex = 0;
    while (listEntry != NULL) {
        BRS_GUI_Widget *widget = listEntry->value;
        BRS_GUI_Widget_calculate(widget);
        BRS_GUI_calculateWidgetList(widget->properties->children, widget);
        listEntry = listEntry->next;
        widgetIndex++;
    }
}

void BRS_GUI_calculate(BRS_GUI *gui) {
    BRS_GUI_calculateWidgetList(gui->widgetList, NULL);
}

void BRS_GUI_render(BRS_GUI *gui, BRS_VideoContext *videoContext) {
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
            if (BRS_GUI_processEventForList(widget->properties->children, event, zIndexHighestVisible, depth)) {
                return true;
            }
            depth--;
        }
        entry = entry->next;
    }
    return false;
}

static void calcZIndexHighestVisible(BRS_GUI_WidgetList *widgetList, int16_t *zIndexHighestVisible, uint8_t depth) {
    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (widget->properties->visible && widget->properties->zIndex > zIndexHighestVisible[depth]) {
            zIndexHighestVisible[depth] = widget->properties->zIndex;
        }
        depth++;
        calcZIndexHighestVisible(widget->properties->children, zIndexHighestVisible, depth);
        depth--;
        entry = entry->next;
    }
}

bool BRS_GUI_processEvent(BRS_GUI *gui, SDL_Event *event) {
    int16_t zIndexHighestVisible[BRS_GUI_WIDGET_DEPTH_MAX] = {0};
    calcZIndexHighestVisible(gui->widgetList, zIndexHighestVisible, 0);
    return BRS_GUI_processEventForList(gui->widgetList, event, zIndexHighestVisible, 0);
}
