#include "font.h"

#include <string.h>

#include "renderer.h"
#include "sprite_sheet.h"

#define FONT_Y_OFFSET 232

static i32 colors[4] = {OPAQUE, OPAQUE, OPAQUE, OPAQUE};

void bitmap_font_init(bitmap_font_t* bitmap_font) {
    memset(bitmap_font, 0, sizeof(bitmap_font_t));

    bitmap_font->char_rows = 16;
    bitmap_font->char_cols = 3;
    bitmap_font->char_width = 8;
    bitmap_font->char_height = 8;

    bitmap_font->chars = string_create("ABCDEFGHIJKLMNOPQRSTUVWXYZ!/<>:+1234567890&=(),.?");
}

void bitmap_font_blit_char(bitmap_font_t* bitmap_font, sprite_sheet_t* sprite_sheet, renderer_t* renderer,
                           char character, i32 ox, i32 oy) {
    for (i32 y = 0; y < bitmap_font->char_cols; y++) {
        for (i32 x = 0; x < bitmap_font->char_rows; x++) {
            if (bitmap_font->chars.data[y * bitmap_font->char_rows + x] != character) {
                continue;
            }

            sprite_sheet_blit_sprite(sprite_sheet, renderer, ox, oy, x * bitmap_font->char_width,
                                     FONT_Y_OFFSET + (y * bitmap_font->char_height), bitmap_font->char_width,
                                     bitmap_font->char_height, colors, FLIP_NONE);
        }
    }
}

void bitmap_font_blit_str(bitmap_font_t* font, sprite_sheet_t* sprite_sheet, renderer_t* renderer, string_t string,
                          i32 ox, i32 oy, i32 fg, i32 bg, u32 flags) {
    if (flags & FONT_FANCY) {
        colors[0] = bg;

        colors[3] = fg - rgb_to_palette(1, 1, 1);
        if (colors[3] < 0) colors[3] = 0;

        for (i32 i = 0; i < string.length; i++) {
            bitmap_font_blit_char(font, sprite_sheet, renderer, string.data[i], ox + i * font->char_width + 1, oy + 1);
        }

        colors[0] = OPAQUE;
        colors[3] = fg;

        for (i32 i = 0; i < string.length; i++) {
            bitmap_font_blit_char(font, sprite_sheet, renderer, string.data[i], ox + i * font->char_width, oy);
        }
    }
    else {
        colors[0] = bg;
        colors[3] = fg;

        for (i32 i = 0; i < string.length; i++) {
            bitmap_font_blit_char(font, sprite_sheet, renderer, string.data[i], ox + i * font->char_width, oy);
        }
    }
}

void bitmap_font_destroy(bitmap_font_t* bitmap_font) {
    bitmap_font->char_rows = 0;
    bitmap_font->char_cols = 0;
    bitmap_font->char_width = 0;
    bitmap_font->char_height = 0;

    string_free(&bitmap_font->chars);
}
