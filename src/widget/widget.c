//
// Abstract widget
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
#include <string.h>
#include "widget.h"

BRS_LIST_DEFN(BRS_GUI_WidgetList, BRS_GUI_Widget)

void BRS_GUI_Widget_ctor(BRS_GUI_Widget *widget, BRS_GUI_Widget_Properties *widgetProps) {
    widget->properties = widgetProps;
    widget->properties->processEventHandler = NULL;
    widget->properties->calculateHandler = NULL;
    widget->properties->destroyHandler = NULL;
    widget->properties->renderHandler = NULL;
    widget->properties->executeActionHandler = NULL;
};

void BRS_GUI_Widget_dtor(BRS_GUI_Widget *widget) {
    BRS_GUI_WidgetListEntry *childListEntry = widget->properties->children->firstEntry;
    while (childListEntry != NULL) {
        BRS_GUI_Widget *widgetChild = childListEntry->value;
        widgetChild->properties->destroyHandler(widgetChild);
        childListEntry = childListEntry->next;
    }
    BRS_GUI_Widget_Properties_destroy(widget->properties);
};

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_create(const char *id) {
    BRS_GUI_Widget_Properties *properties = malloc(sizeof(BRS_GUI_Widget_Properties));

    BRS_Point position = {.x = 0, .y = 0};
    properties->position = BRS_copyPoint(&position);

    BRS_Size size = {.width = 0, .height = 0};
    properties->size = BRS_copySize(&size);

    BRS_Padding padding = {.left = 0, .right = 0, .top = 0, .bottom = 0};
    properties->padding = BRS_copyPadding(&padding);

    properties->theme = NULL;
    properties->zIndex = 0;
    properties->visible = false;
    properties->id = strdup(id);

    properties->parent = NULL;
    properties->children = BRS_GUI_WidgetList_create();

    properties->clickHandler = NULL;
    properties->calculateHandler = NULL;
    properties->executeActionHandler = NULL;
    properties->processEventHandler = NULL;
    properties->renderHandler = NULL;

    return properties;
}

BRS_GUI_Widget_Properties *BRS_GUI_Widget_Properties_copy(BRS_GUI_Widget_Properties *source) {
    BRS_GUI_Widget_Properties *properties = malloc(sizeof(BRS_GUI_Widget_Properties));

    properties->position = BRS_copyPoint(source->position);
    properties->size = BRS_copySize(source->size);
    properties->padding = BRS_copyPadding(source->padding);
    properties->theme = source->theme;
    properties->zIndex = source->zIndex;
    properties->visible = source->visible;
    properties->parent = source->parent;
    properties->children = BRS_GUI_WidgetList_create();
    BRS_GUI_WidgetListEntry *childEntry = source->children->firstEntry;
    while (childEntry != NULL) {
        BRS_GUI_WidgetList_push(childEntry->value, properties->children);
        childEntry = childEntry->next;
    }
    properties->id = source->id != NULL ? strdup(source->id) : NULL;
    properties->renderHandler = source->renderHandler;
    properties->processEventHandler = source->processEventHandler;
    properties->executeActionHandler = source->executeActionHandler;
    properties->calculateHandler = source->calculateHandler;
    properties->clickHandler = source->clickHandler;

    return properties;
}

void BRS_GUI_Widget_Properties_destroy(BRS_GUI_Widget_Properties *properties) {
    free(properties->padding);
    free(properties->position);
    free(properties->size);
    free(properties->id);
    BRS_GUI_WidgetList_destroy(properties->children);
    free(properties);
}

BRS_GUI_Widget *BRS_GUI_Widget_findRootWidget(BRS_GUI_Widget *widget) {
    if (widget == NULL) {
        return NULL;
    }
    BRS_GUI_Widget *currentWidget = widget;
    while (currentWidget->properties->parent != NULL) {
        currentWidget = currentWidget->properties->parent;
    }
    return currentWidget;
}

BRS_GUI_Widget *BRS_GUI_Widget_getByIdFromList(const char *id, BRS_GUI_WidgetList *widgetList) {
    if (id == NULL || widgetList == NULL) {
        return NULL;
    }

    BRS_GUI_WidgetListEntry *entry = widgetList->firstEntry;
    while (entry != NULL) {
        BRS_GUI_Widget *widget = entry->value;
        if (strcmp(widget->properties->id, id) == 0) {
            return widget;
        }
        BRS_GUI_Widget *widgetFound = BRS_GUI_Widget_getByIdFromList(id, widget->properties->children);
        if (widgetFound != NULL) {
            return widgetFound;
        }
        entry = entry->next;
    }
    return NULL;
}

BRS_GUI_Widget *BRS_GUI_Widget_findWidgetById(const char *id, BRS_GUI_Widget *rootWidget) {
    if (id == NULL || rootWidget == NULL) {
        return NULL;
    }

    if (strcmp(rootWidget->properties->id, id) == 0) {
        return rootWidget;
    } else {
        return BRS_GUI_Widget_getByIdFromList(id, rootWidget->properties->children);
    }
}

bool BRS_GUI_Widget_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    if (widget->properties->processEventHandler != NULL) {
        return widget->properties->processEventHandler(widget, event);
    } else {
        return false;
    }
}

void BRS_GUI_Widget_render(BRS_GUI_Widget *widget, BRS_VideoContext *context) {
    if (widget->properties->renderHandler != NULL) {
        widget->properties->renderHandler(widget, context);
    }
}

void BRS_GUI_Widget_destroy(BRS_GUI_Widget *widget) {
    if (widget->properties->destroyHandler != NULL) {
        widget->properties->destroyHandler(widget);
    }
}

void BRS_GUI_Widget_calculate(BRS_GUI_Widget *widget) {
    if (widget->properties->calculateHandler != NULL) {
        widget->properties->calculateHandler(widget);
    }
}

void BRS_GUI_Widget_setVisible(BRS_GUI_Widget *widget, bool visible) {
    widget->properties->visible = visible;
}
