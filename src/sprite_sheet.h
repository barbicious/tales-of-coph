#pragma once

#include "utils.h"

#define OPAQUE 255

#define FLIP_NONE (1 << 0)
#define FLIP_HORIZONTAL (1 << 1)
#define FLIP_VERTICAL (1 << 2)

typedef struct renderer renderer_t;
typedef struct bitmap_font bitmap_font_t;

typedef struct sprite_sheet {
    i32 width, height;
    u8* pixels;
} sprite_sheet_t;

void sprite_sheet_init(sprite_sheet_t* sprite_sheet, string_t file_path);
void sprite_sheet_blit_sprite(sprite_sheet_t* sprite_sheet, renderer_t* renderer, i32 ox, i32 oy, u32 sx, u32 sy,
                              u32 width, u32 height, i32 colors[], u32 flags);
void sprite_sheet_blit_frame(sprite_sheet_t* sprite_sheet, renderer_t* renderer, i32 ox, i32 oy,
                             u32 width, u32 height, u32 flags);
void sprite_sheet_blit_text_frame(sprite_sheet_t* sprite_sheet, renderer_t* renderer, bitmap_font_t* bitmap_font,
                                  i32 ox, i32 oy,
                                  u32 width, u32 height, i32 fg, i32 bg, u32 flags, string_t text);
void sprite_sheet_destroy(sprite_sheet_t* sprite_sheet);
