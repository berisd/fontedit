//
// Created by bernd on 05.01.21.
//
#include "gui.h"
#include "widget/widgets.h"

static const char *WIDGET_ID_MENU_FILE = "MENU_FILE";
static const char *WIDGET_ID_MENU_BAR = "MENU_BAR";
static const char *WIDGET_ID_LABEL_STATUS = "LABEL_STATUS";
static const char *WIDGET_ID_CHAR_TABLE = "CHAR_TABLE";
static const char *WIDGET_ID_CHAR_EDIT = "CHAR_EDIT";
static const char *WIDGET_ID_INPUT_BOX = "INPUT_BOX";
static const char *WIDGET_ID_WINDOW_MAIN = "WINDOW_MAIN";

static void onClickCharTable(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *charEditWidget = BRS_GUI_Widget_findWidgetById(WIDGET_ID_CHAR_EDIT, rootWidget);
    BRS_GUI_CharTable *charTable = widget->object;
    ((BRS_GUI_CharEdit *) charEditWidget->object)->selectedChar = charTable->selectedCharIndex;
}

static void onConfirmInputBoxLoad(BRS_GUI_Widget *widget) {
    BRS_GUI_InputBox *inputBox = widget->object;
    bool success = BRS_FontEdit_loadFont(inputBox->text);
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *label = BRS_GUI_Widget_findWidgetById(WIDGET_ID_LABEL_STATUS, rootWidget);
    if (success) {
        char str[255];
        sprintf(str, "Font %s loaded successfully!", inputBox->text);
        BRS_GUI_Label_setText(label, str);
    } else {
        BRS_GUI_Label_setText(label, "Error loading font!");
    }
}

static void onConfirmInputBoxSave(BRS_GUI_Widget *widget) {
    BRS_GUI_InputBox *inputBox = widget->object;
    bool success = BRS_FontEdit_saveFont(inputBox->text);
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *label = BRS_GUI_Widget_findWidgetById(WIDGET_ID_LABEL_STATUS, rootWidget);
    if (success) {
        char str[255];
        sprintf(str, "Font %s saved successfully!", inputBox->text);
        BRS_GUI_Label_setText(label, str);
    } else {
        BRS_GUI_Label_setText(label, "Error saving font!");
    }
}

static void onCancelInputBox(BRS_GUI_Widget *widget) {
}

static void onChangeCharTableSelectedCharIndex(BRS_GUI_Widget *widget) {
    BRS_GUI_CharTable *charTable = widget->object;
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *charEditWidget = BRS_GUI_Widget_findWidgetById(WIDGET_ID_CHAR_EDIT, rootWidget);
    ((BRS_GUI_CharEdit *) charEditWidget->object)->selectedChar = charTable->selectedCharIndex;
}

static void onClickMenuItemLoad(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *inputBoxWidget = BRS_GUI_Widget_findWidgetById(WIDGET_ID_INPUT_BOX, rootWidget);
    BRS_GUI_InputBox *inputBox = inputBoxWidget->object;
    BRS_GUI_InputBox_setConfirmHandler(inputBoxWidget, onConfirmInputBoxLoad);
    inputBox->title = "Load file";
    inputBox->textLabel = "Name?";
    BRS_GUI_InputBox_clearText(inputBoxWidget);
    inputBoxWidget->properties->visible = true;
}

static void onClickMenuItemSave(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *inputBoxWidget = BRS_GUI_Widget_findWidgetById(WIDGET_ID_INPUT_BOX, rootWidget);
    BRS_GUI_InputBox *inputBox = inputBoxWidget->object;
    BRS_GUI_InputBox_setConfirmHandler(inputBoxWidget, onConfirmInputBoxSave);
    inputBox->title = "Save file";
    inputBox->textLabel = "Name?";
    BRS_GUI_InputBox_clearText(inputBoxWidget);
    inputBoxWidget->properties->visible = true;
}

static void onClickMenuItemNew(BRS_GUI_Widget *widget) {
    BRS_GUI_Widget *rootWidget = BRS_GUI_Widget_findRootWidget(widget);
    BRS_GUI_Widget *label = BRS_GUI_Widget_findWidgetById(WIDGET_ID_LABEL_STATUS, rootWidget);
    BRS_FontEdit_createFont();
    BRS_GUI_Label_setText(label, "New font created!");
    BRS_GUI_Widget *charTableWidget = BRS_GUI_Widget_findWidgetById(WIDGET_ID_CHAR_TABLE, rootWidget);
    BRS_GUI_CharTable_setSelectedCharIndex(charTableWidget, 0);
}

static void onClickMenuItemQuit(BRS_GUI_Widget *widget) {
    BRS_FontEdit_quitApplication();
}

static BRS_GUI_Widget *createFileMenu(BRS_GUI_Widget *menuBarWidget) {
    BRS_GUI_Widget_Properties *widgetFileMenuProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_MENU_FILE);
    widgetFileMenuProps->size->width = 100;
    widgetFileMenuProps->size->height = 20;
    widgetFileMenuProps->padding->left = 5;
    widgetFileMenuProps->theme = menuBarWidget->properties->theme;

    BRS_GUI_Widget *fileMenuWidget = BRS_GUI_Widget_createMenu(widgetFileMenuProps, BRS_GUI_Menu_create("File"),
                                                               menuBarWidget);

    BRS_GUI_Widget *menuItemNew = BRS_GUI_Widget_createMenuItem(BRS_GUI_Widget_Properties_copy(widgetFileMenuProps),
                                                                BRS_GUI_MenuItem_create("New"), fileMenuWidget);
    BRS_GUI_Widget *menuItemLoad = BRS_GUI_Widget_createMenuItem(BRS_GUI_Widget_Properties_copy(widgetFileMenuProps),
                                                                 BRS_GUI_MenuItem_create("Load"), fileMenuWidget);
    BRS_GUI_Widget *menuItemSave = BRS_GUI_Widget_createMenuItem(BRS_GUI_Widget_Properties_copy(widgetFileMenuProps),
                                                                 BRS_GUI_MenuItem_create("Save"), fileMenuWidget);
    BRS_GUI_Widget *menuItemQuit = BRS_GUI_Widget_createMenuItem(BRS_GUI_Widget_Properties_copy(widgetFileMenuProps),
                                                                 BRS_GUI_MenuItem_create("Quit"), fileMenuWidget);

    BRS_GUI_Widget_setClickHandler(menuItemNew, onClickMenuItemNew);
    BRS_GUI_Widget_setClickHandler(menuItemLoad, onClickMenuItemLoad);
    BRS_GUI_Widget_setClickHandler(menuItemSave, onClickMenuItemSave);
    BRS_GUI_Widget_setClickHandler(menuItemQuit, onClickMenuItemQuit);

    return fileMenuWidget;
}

static BRS_GUI_Widget *createMenuBar(BRS_GUI_Theme *theme, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_MENU_BAR);
    widgetProps->size->width = 800;
    widgetProps->size->height = 20;
    widgetProps->theme = theme;
    widgetProps->visible = true;
    BRS_GUI_Widget *menuBarWidget = BRS_GUI_Widget_createMenuBar(widgetProps, parent);

    BRS_GUI_WidgetList_push(createFileMenu(menuBarWidget), menuBarWidget->children);

    return menuBarWidget;
}

static BRS_GUI_Widget *createLabelStatus(BRS_GUI_Theme *theme, BRS_Size *windowSize, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_LABEL_STATUS);
    widgetProps->position->x = 1;
    widgetProps->position->y = windowSize->height - theme->font->height_bits - 1;
    widgetProps->theme = theme;
    widgetProps->visible = true;

    const char *labelText = "Ready.";
    BRS_GUI_Widget *widget = BRS_GUI_Widget_createLabel(widgetProps, labelText, parent);
    return widget;
}

static BRS_GUI_Widget *createCharTable(BRS_GUI_Theme *theme, BRS_Font *fontEdited, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_CHAR_TABLE);
    widgetProps->position->x = 10;
    widgetProps->position->y = 60;
    widgetProps->theme = theme;
    widgetProps->visible = true;

    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharTable(widgetProps, fontEdited, parent);
    BRS_GUI_Widget_setClickHandler(widget, onClickCharTable);
    BRS_GUI_CharTable_setChangedSelectedCharIndexHandler(widget, onChangeCharTableSelectedCharIndex);
    return widget;
}

static BRS_GUI_Widget *createCharEdit(BRS_GUI_Theme *theme, BRS_Font *fontEdited, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_CHAR_EDIT);
    widgetProps->position->x = 500;
    widgetProps->position->y = 50;
    widgetProps->theme = theme;
    widgetProps->visible = true;

    BRS_GUI_Widget *widget = BRS_GUI_Widget_createCharEdit(widgetProps, fontEdited, parent);

    return widget;
}

static BRS_GUI_Widget *createInputBox(BRS_GUI_Theme *theme, BRS_Size *windowSize, BRS_GUI_Widget *parent) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_INPUT_BOX);
    widgetProps->size->width = 200;
    widgetProps->size->height = 100;
    widgetProps->position->x = (windowSize->width - widgetProps->size->width) / 2;
    widgetProps->position->y = (windowSize->height - widgetProps->size->height) / 2 - widgetProps->size->height;
    widgetProps->theme = theme;
    widgetProps->zIndex = 100;

    BRS_GUI_Widget *widget = BRS_GUI_Widget_createInputBox(widgetProps, "Input", "?", parent);
    BRS_GUI_InputBox_setCancelHandler(widget, onCancelInputBox);
    return widget;
}

static BRS_GUI_Widget *
createMainWindow(BRS_GUI_Theme *theme, uint32_t screenWidth, uint32_t screenHeight) {
    BRS_GUI_Widget_Properties *widgetProps = BRS_GUI_Widget_Properties_create(WIDGET_ID_WINDOW_MAIN);
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

    BRS_GUI_WidgetList_push(createCharTable(theme, fontEdited, window), window->children);
    BRS_GUI_WidgetList_push(createCharEdit(theme, fontEdited, window), window->children);
    BRS_GUI_WidgetList_push(createLabelStatus(theme, window->properties->size, window), window->children);
    BRS_GUI_WidgetList_push(createMenuBar(theme, window), window->children);
    BRS_GUI_WidgetList_push(createInputBox(theme, window->properties->size, window), window->children);

    BRS_GUI *gui = malloc(sizeof(BRS_GUI));
    gui->widgetList = widgetList;

    return gui;
}

void BRS_GUI_init(BRS_GUI *gui) {
    BRS_GUI_Widget *charTableWidget = BRS_GUI_Widget_getByType(BRS_GUI_WIDGET_CHARTABLE, gui->widgetList);
    BRS_GUI_CharTable_setSelectedCharIndex(charTableWidget, 0);
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
