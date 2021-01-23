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

typedef struct _BRS_Font_LoadResult {
    BRS_Font *font;
    BRS_FontError error;
} BRS_Font_LoadResult;

typedef struct _BRS_Font_SaveResult {
    BRS_FontError error;
} BRS_Font_SaveResult;

BRS_Font_LoadResult *BRS_Font_load(const char *filename);
BRS_Font_SaveResult *BRS_Font_save(const BRS_Font *font, const char *filename);
void BRS_Font_destroyLoadResult(BRS_Font_LoadResult * result);
void BRS_Font_destroySaveResult(BRS_Font_SaveResult * result);
BRS_Font *BRS_Font_create(uint16_t num_chars, uint8_t width_bits, uint8_t height_bits, uint8_t *data);
void BRS_Font_destroy(BRS_Font *font);
int32_t BRS_Font_getSize(BRS_Font *font);
BRS_Font *BRS_Font_copy(BRS_Font *font);

#endif //SDLSCROLLTEXTMUS_FONT_H
