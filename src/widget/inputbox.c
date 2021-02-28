//
// Created by bernd on 10.01.21.
//

#include "inputbox.h"

static const uint8_t MAX_TEXT_LEN = 100;

static void BRS_GUI_InputBox_render(const BRS_GUI_Widget *widget, const BRS_VideoContext *context) {
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    BRS_GUI_InputBox *inputBox = (BRS_GUI_InputBox *) widget;
    if (widgetProps->visible) {
        BRS_Rect rect = {.x = widgetProps->position->x, .y = widgetProps->position->y,
                .width = widgetProps->size->width, .height = widgetProps->size->height};
        BRS_setColor(context, widgetProps->theme->inputBoxBackColor);
        BRS_drawlFillRect(context, &rect);
        BRS_setColor(context, widgetProps->theme->borderColor);
        BRS_drawlRect(context, &rect);

        BRS_Point titlePoint = {.x = widgetProps->position->x + 1, .y = widgetProps->position->y + 1};
        BRS_setColor(context, widgetProps->theme->inputBoxTitleForeColor);
        BRS_drawString(context, inputBox->title, strlen(inputBox->title), widgetProps->theme->font, &titlePoint);

        BRS_Point titleLineP1 = {.x = widgetProps->position->x + 1, .y = widgetProps->position->y + 1 + 20};
        BRS_Point titleLineP2 = {.x = widgetProps->position->x + widgetProps->size->width, .y =
        widgetProps->position->y + 1 + 20};
        BRS_Line titleLine = {.p1 = &titleLineP1, .p2 = &titleLineP2};
        BRS_setColor(context, widgetProps->theme->borderColor);
        BRS_drawline(context, &titleLine);

        BRS_Point textLabelPoint = {.x = widgetProps->position->x + 1, .y = widgetProps->position->y + 1 + 25};
        BRS_setColor(context, widgetProps->theme->inputBoxTextForeColor);
        BRS_drawString(context, inputBox->textLabel, strlen(inputBox->textLabel), widgetProps->theme->font,
                       &textLabelPoint);

        BRS_Point textBeginPoint = {.x = widgetProps->position->x + 50, .y = widgetProps->position->y + 1 + 25};
        BRS_drawString(context, inputBox->text, strlen(inputBox->text), widgetProps->theme->font, &textBeginPoint);

        uint32_t textWidth = strlen(inputBox->text) * (widgetProps->theme->font->width_bits + 1);

        BRS_Point cursorBeginPoint = {.x = textBeginPoint.x + textWidth, .y = widgetProps->position->y + 1 + 25};
        BRS_Point cursorEndPoint = {.x = cursorBeginPoint.x, .y = cursorBeginPoint.y +
                                                                  widgetProps->theme->font->height_bits};
        BRS_Line cursorLine = {.p1 = &cursorBeginPoint, .p2 = &cursorEndPoint};
        BRS_drawline(context, &cursorLine);
    }
}

static bool isKeycodeAlphaNumeric(SDL_Keycode keycode) {
    return keycode == SDLK_0 || keycode == SDLK_1 || keycode == SDLK_2 || keycode == SDLK_3 || keycode == SDLK_4 ||
           keycode == SDLK_5 || keycode == SDLK_6 || keycode == SDLK_7 || keycode == SDLK_8 || keycode == SDLK_9 ||
           keycode == SDLK_UNDERSCORE || keycode == SDLK_PERIOD || keycode == SDLK_KP_PERIOD || keycode == SDLK_a ||
           keycode == SDLK_b || keycode == SDLK_c || keycode == SDLK_d || keycode == SDLK_e || keycode == SDLK_f ||
           keycode == SDLK_g || keycode == SDLK_h || keycode == SDLK_i || keycode == SDLK_j || keycode == SDLK_k ||
           keycode == SDLK_l || keycode == SDLK_m || keycode == SDLK_n || keycode == SDLK_o || keycode == SDLK_p ||
           keycode == SDLK_q || keycode == SDLK_r || keycode == SDLK_s || keycode == SDLK_t || keycode == SDLK_u ||
           keycode == SDLK_v || keycode == SDLK_w || keycode == SDLK_x || keycode == SDLK_y || keycode == SDLK_z;
}

static bool BRS_GUI_InputBox_processEvent(BRS_GUI_Widget *widget, SDL_Event *event) {
    BRS_GUI_InputBox *inputBox = (BRS_GUI_InputBox *) widget;
    BRS_GUI_Widget_Properties *widgetProps = widget->properties;
    if (!widgetProps->visible) {
        return false;
    }

    if (event->type == SDL_KEYDOWN) {
        size_t inputTextLength = strlen(inputBox->text);
        if (event->key.keysym.sym == SDLK_BACKSPACE) {
            if (inputTextLength > 0) {
                inputBox->text[inputTextLength - 1] = 0x0;
            }
        } else if (isKeycodeAlphaNumeric(event->key.keysym.sym)) {
            if (inputTextLength < MAX_TEXT_LEN) {
                inputBox->text[inputTextLength] = event->key.keysym.sym;
            }
        } else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER) {
            widgetProps->visible = false;
            if (inputBox->confirmHandler != NULL) {
                inputBox->confirmHandler(inputBox);
            }
        } else if (event->key.keysym.sym == SDLK_ESCAPE) {
            widgetProps->visible = false;
            if (inputBox->cancelHandler != NULL) {
                inputBox->cancelHandler(inputBox);
            }
            return true;
        }
    }
    return false;
}

void BRS_GUI_InputBox_ctor(BRS_GUI_InputBox *inputBox, BRS_GUI_Widget_Properties *widgetProps, const char *title,
                           const char *textLabel) {
    BRS_GUI_Widget_ctor((BRS_GUI_Widget *) inputBox, widgetProps);
    inputBox->title = strdup(title);
    inputBox->textLabel = strdup(textLabel);
    inputBox->text = malloc(MAX_TEXT_LEN);
    widgetProps->processEventHandler = BRS_GUI_InputBox_processEvent;
    widgetProps->renderHandler = BRS_GUI_InputBox_render;
    inputBox->cancelHandler = NULL;
    inputBox->confirmHandler = NULL;
    widgetProps->destroyHandler = (BRS_GUI_Widget_DestroyHandler) BRS_GUI_InputBox_destroy;
}

void BRS_GUI_InputBox_dtor(BRS_GUI_InputBox *inputBox) {
    BRS_GUI_Widget_dtor((BRS_GUI_Widget *) inputBox);
    free(inputBox->title);
    free(inputBox->textLabel);
    free(inputBox->text);
}

BRS_GUI_InputBox *
BRS_GUI_InputBox_create(BRS_GUI_Widget_Properties *widgetProps, const char *title, const char *textLabel) {
    BRS_GUI_InputBox *inputBox = malloc(sizeof(BRS_GUI_InputBox));
    BRS_GUI_InputBox_ctor(inputBox, widgetProps, title, textLabel);
    return inputBox;
}

void BRS_GUI_InputBox_destroy(BRS_GUI_InputBox *inputBox) {
    BRS_GUI_InputBox_dtor(inputBox);
    free(inputBox);
}


void BRS_GUI_InputBox_clearText(BRS_GUI_InputBox *inputBox) {
    memset(inputBox->text, 0, MAX_TEXT_LEN);
}

void BRS_GUI_InputBox_setConfirmHandler(BRS_GUI_InputBox *inputBox, BRS_GUI_InputBox_ConfirmHandler handler) {
    inputBox->confirmHandler = handler;
}

void BRS_GUI_InputBox_setCancelHandler(BRS_GUI_InputBox *inputBox, BRS_GUI_InputBox_CancelHandler handler) {
    inputBox->cancelHandler = handler;
}

void BRS_GUI_InputBox_setTitle(BRS_GUI_InputBox *inputBox, const char *title) {
    strcpy(inputBox->title, title);
}

void BRS_GUI_InputBox_setTextLabel(BRS_GUI_InputBox *inputBox, const char *textLabel) {
    strcpy(inputBox->textLabel, textLabel);
}
