#ifndef SDLSCROLLTEXTMUS_FONT_H
#define SDLSCROLLTEXTMUS_FONT_H
#include <stdint.h>

typedef struct BRS_Font BRS_Font;
struct BRS_Font {
    uint8_t *data;
    uint8_t height_bits;
    uint8_t width_bits;
    uint16_t num_chars;
};

typedef enum BRS_FontError BRS_FontError;
enum BRS_FontError {
    BRS_FONT_NO_ERROR,
    BRS_FONT_ERR_OPENING_FILE,
    BRS_FONT_ERR_READING_FILE,
    BRS_FONT_ERR_WRITING_FILE
};

typedef struct _BRS_LoadFontResult {
    BRS_Font *font;
    BRS_FontError error;
} BRS_LoadFontResult;

typedef struct _BRS_SaveFontResult {
    BRS_FontError error;
} BRS_SaveFontResult;

BRS_LoadFontResult *BRS_loadFont(const char *filename);
BRS_SaveFontResult *BRS_saveFont(const BRS_Font *font, const char *filename);
void BRS_destroyLoadFontResult(BRS_LoadFontResult * result);
void BRS_destroySaveFontResult(BRS_SaveFontResult * result);
BRS_Font *BRS_createFont(uint16_t num_chars, uint8_t width_bits, uint8_t height_bits, uint8_t *data);
void BRS_destroyFont(BRS_Font *font);
int32_t BRS_getFontSize(BRS_Font *font);
BRS_Font *BRS_copyFont(BRS_Font *font);

#endif //SDLSCROLLTEXTMUS_FONT_H
