#include <malloc.h>
#include <string.h>
#include "font.h"

BRS_LoadFontResult *BRS_loadFont(const char *filename) {
    BRS_LoadFontResult *result = malloc(sizeof(BRS_LoadFontResult));
    result->error = BRS_FONT_NO_ERROR;
    FILE *fontFile = fopen(filename, "rb");
    if (!fontFile) {
        result->error = BRS_FONT_ERR_OPENING_FILE;
    } else {
        BRS_Font *font = malloc(sizeof(BRS_Font));
        font->width_bits = 8;
        font->height_bits = 16;
        font->num_chars = 256;
        result->font = font;

        uint8_t fontCharSizeBytes = font->width_bits * font->height_bits / 8;
        uint32_t fontSizeBytes = fontCharSizeBytes * font->num_chars;

        font->data = malloc(fontSizeBytes);
        size_t bytesRead = fread(font->data, sizeof(uint8_t), fontSizeBytes, fontFile);

        if (ferror(fontFile) || bytesRead < fontSizeBytes) {
            result->error = BRS_FONT_ERR_READING_FILE;
        }

        fclose(fontFile);
    }

    return result;
}

BRS_Font *BRS_createFont(uint16_t num_chars, uint8_t width_bits, uint8_t height_bits, uint8_t *data) {
    BRS_Font *font = malloc(sizeof(BRS_Font));
    font->width_bits = width_bits;
    font->height_bits = height_bits;
    font->num_chars = num_chars;
    font->data = data;
    return font;
}

void BRS_destroyFont(BRS_Font *font) {
    if (font->data) {
        free(font->data);
    }
    free(font);
}

int32_t BRS_getFontSize(BRS_Font *font) {
    return font->width_bits / 8 * font->height_bits * font->num_chars;
}

BRS_Font *BRS_copyFont(BRS_Font *font) {
    BRS_Font *copy = malloc(sizeof(BRS_Font));
    copy->width_bits = font->width_bits;
    copy->height_bits = font->height_bits;
    copy->num_chars = font->num_chars;
    size_t fontSize = BRS_getFontSize(font);
    copy->data = malloc(fontSize);
    memcpy(copy->data, font->data, fontSize);
    return font;
}
