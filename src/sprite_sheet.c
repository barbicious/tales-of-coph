#include "sprite_sheet.h"

#include <assert.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "renderer.h"
#include "stb_image.h"

#define SPRITE_SHEET_PIXEL(x, y) (sprite_sheet->pixels[((y) * sprite_sheet->width + (x)) * 4])

void sprite_sheet_init(sprite_sheet_t* sprite_sheet, string_t file_path) {
    i32 width, height, channels;

    FILE* file = fopen(file_path.data, "rb");

    u8* pixels = stbi_load_from_file(file, &width, &height, &channels, 4);

    fclose(file);

    assert(pixels != NULL);

    sprite_sheet->pixels = pixels;
    sprite_sheet->width = width;
    sprite_sheet->height = height;

    for (i32 y = 0; y < height; y++) {
        for (i32 x = 0; x < width; x++) {
            if (SPRITE_SHEET_PIXEL(x, y) == 0) {
                SPRITE_SHEET_PIXEL(x, y) = OPAQUE;
                continue;
            }

            SPRITE_SHEET_PIXEL(x, y) /= 64;
        }
    }

    string_free(&file_path);
}

void sprite_sheet_blit_sprite(sprite_sheet_t* sprite_sheet, renderer_t* renderer, i32 ox, i32 oy, u32 sx, u32 sy, u32 width, u32 height, i32 colors[]) {
    for (i32 y = 0; y < height; y++) {
        i32 py = oy + y;

        if (py < 0 || py >= SCREEN_HEIGHT) {
            continue;
        }

        for (i32 x = 0; x < width; x++) {
            i32 px = ox + x;

            if (px < 0 || px >= SCREEN_WIDTH) {
                continue;
            }

            if (SPRITE_SHEET_PIXEL(sx + x, sy + y) == OPAQUE) {
                continue;
            }

            renderer_set_pixel(renderer, px, py, colors[SPRITE_SHEET_PIXEL(x + sx, y + sy)]);
        }
    }
}

void sprite_sheet_destroy(sprite_sheet_t* sprite_sheet) {
    stbi_image_free(sprite_sheet->pixels);
}