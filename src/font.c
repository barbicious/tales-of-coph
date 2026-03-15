#include "font.h"

#include <stdio.h>
#include <string.h>

#include "renderer.h"
#include "sprite_sheet.h"

void bitmap_font_init(bitmap_font_t* bitmap_font) {
    memset(bitmap_font, 0, sizeof(bitmap_font_t));

    bitmap_font->char_rows = 16;
    bitmap_font->char_cols = 3;
    bitmap_font->char_width = 8;
    bitmap_font->char_height = 8;

    bitmap_font->chars = string_new("ABCDEFGHIJKLMNOPQRSTUVWXYZ!/<>:+1234567890&=(),.?");
}

void bitmap_font_blit_char(bitmap_font_t* bitmap_font, sprite_sheet_t* sprite_sheet, renderer_t* renderer, char character, i32 ox, i32 oy, i32 fg, i32 bg) {
    for (i32 y = 0; y < bitmap_font->char_cols; y++) {
        for (i32 x = 0; x < bitmap_font->char_rows; x++) {
            if (bitmap_font->chars.data[y * bitmap_font->char_rows + x] != character) {
                continue;
            }

            i32 colors[] = {bg, OPAQUE, OPAQUE, fg};

            sprite_sheet_blit_sprite(sprite_sheet, renderer, ox, oy, x * bitmap_font->char_width, 232 + (y * bitmap_font->char_height), bitmap_font->char_width, bitmap_font->char_height, colors);
        }
    }
}

void bitmap_font_blit_str(bitmap_font_t* font, sprite_sheet_t* sprite_sheet, renderer_t* renderer, string_t string, i32 ox, i32 oy, i32 fg, i32 bg) {
    for (i32 i = 0; i < string.length; i++) {
        bitmap_font_blit_char(font, sprite_sheet, renderer, string.data[i], ox + i * font->char_width, oy, fg, bg);
    }
}

void bitmap_font_destroy(bitmap_font_t* bitmap_font) {
    bitmap_font->char_rows = 0;
    bitmap_font->char_cols = 0;
    bitmap_font->char_width = 0;
    bitmap_font->char_height = 0;

    string_free(&bitmap_font->chars);
}
