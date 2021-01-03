#include <malloc.h>
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

void BRS_freeFont(BRS_Font *font) {
    if (font->data) {
        free(font->data);
    }
    free(font);
}
