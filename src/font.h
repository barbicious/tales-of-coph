#pragma once

#include "utils.h"

#define FONT_NORMAL (1 << 0)
#define FONT_FANCY (1 << 1)


typedef struct renderer renderer_t;
typedef struct sprite_sheet sprite_sheet_t;

typedef struct bitmap_font {
    u8 char_width, char_height, char_rows, char_cols;
    string_t chars;
} bitmap_font_t;

void bitmap_font_init(bitmap_font_t* font);
void bitmap_font_blit_char(bitmap_font_t* font, sprite_sheet_t* sprite_sheet, renderer_t* renderer, char character,
                           i32 ox, i32 oy);
void bitmap_font_blit_str(bitmap_font_t* font, sprite_sheet_t* sprite_sheet, renderer_t* renderer, string_t string,
                          i32 ox, i32 oy, i32 fg, i32 bg, u32 flags);
void bitmap_font_destroy(bitmap_font_t* bitmap_font);
