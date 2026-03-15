#pragma once

#include "utils.h"

#define OPAQUE 255

typedef struct renderer renderer_t;

typedef struct sprite_sheet {
    i32 width, height;
    u8* pixels;
} sprite_sheet_t;

void sprite_sheet_init(sprite_sheet_t* sprite_sheet, string_t file_path);
void sprite_sheet_blit_sprite(sprite_sheet_t* sprite_sheet, renderer_t* renderer, i32 ox, i32 oy, u32 sx, u32 sy, u32 width, u32 height, i32 colors[]);
void sprite_sheet_destroy(sprite_sheet_t* sprite_sheet);