#include "sprite_sheet.h"

#include <assert.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "font.h"
#include "renderer.h"
#include "stb_image.h"

#define SPRITE_SHEET_PIXEL(x, y) (sprite_sheet->pixels[((y) * sprite_sheet->width + (x)) * 4])

void sprite_sheet_init(sprite_sheet_t* sprite_sheet, string_t file_path) {
    i32 width, height, channels;

    FILE* file = fopen(file_path.data, "rb");

    if (file == nullptr) {
        printf("Failed to load %s", file_path.data);
        exit(0);
    }

    u8* pixels = stbi_load_from_file(file, &width, &height, &channels, 4);

    fclose(file);

    assert(pixels != nullptr);

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

void sprite_sheet_blit_sprite(sprite_sheet_t* sprite_sheet, renderer_t* renderer, i32 ox, i32 oy, u32 sx, u32 sy,
                              u32 width, u32 height, i32 colors[], u32 flags) {
    if (flags == FLIP_NONE) {
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

                if (SPRITE_SHEET_PIXEL(sx + x, sy + y) == OPAQUE || colors[SPRITE_SHEET_PIXEL(x + sx, y + sy)] ==
                    OPAQUE) {
                    continue;
                }

                renderer_set_pixel(renderer, px, py, colors[SPRITE_SHEET_PIXEL(x + sx, y + sy)]);
            }
        }
    }
    else if (flags == FLIP_HORIZONTAL) {
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

                if (SPRITE_SHEET_PIXEL(width - x + sx - 1, sy + y) == OPAQUE || colors[SPRITE_SHEET_PIXEL(
                    width - x + sx - 1, y + sy)] == OPAQUE) {
                    continue;
                }

                renderer_set_pixel(renderer, px, py, colors[SPRITE_SHEET_PIXEL(width - x + sx - 1, y + sy)]);
            }
        }
    }
    else if (flags == FLIP_VERTICAL) {
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

                if (SPRITE_SHEET_PIXEL(x + sx, height - y + sy - 1) == OPAQUE || colors[SPRITE_SHEET_PIXEL(
                    x + sx, height - y + sy - 1)] == OPAQUE) {
                    continue;
                }

                renderer_set_pixel(renderer, px, py, colors[SPRITE_SHEET_PIXEL(x + sx, height - y + sy - 1)]);
            }
        }
    }
}

void sprite_sheet_blit_frame(sprite_sheet_t* sprite_sheet, renderer_t* renderer, i32 ox, i32 oy,
                             u32 width, u32 height, u32 flags) {
    for (usize frame_y = 0; frame_y < height; frame_y++) {
        for (usize frame_x = 0; frame_x < width; frame_x++) {
            i32 sx = 0, sy = 208;

            if (frame_x == 0) {
                sx = 0;
            }
            else if (frame_x == width - 1) {
                sx = 16;
            }
            else {
                sx = 8;
            }

            if (frame_y == 0) {
                sy += 0;
            }
            else if (frame_y == height - 1) {
                sy += 16;
            }
            else {
                sy += 8;
            }

            i32 colors[] = {
                OPAQUE,
                rgb_to_palette(2, 2, 2),
                rgb_to_palette(3, 2, 1),
                rgb_to_palette(4, 4, 4),
            };

            sprite_sheet_blit_sprite(sprite_sheet, renderer, ox + frame_x * 8, oy + frame_y * 8, sx, sy, 8, 8, colors,
                                     flags);
        }
    }
}

void sprite_sheet_blit_text_frame(sprite_sheet_t* sprite_sheet, renderer_t* renderer, bitmap_font_t* bitmap_font,
                                  i32 ox, i32 oy, u32 width,
                                  u32 height, i32 fg, i32 bg, u32 flags, string_t text) {
    sprite_sheet_blit_frame(sprite_sheet, renderer, ox, oy, text.length + 2 + width, height, flags);

    bitmap_font_blit_str(bitmap_font, sprite_sheet, renderer, text, ox + 8, 0, fg, bg, FONT_NORMAL);
}

void sprite_sheet_destroy(sprite_sheet_t* sprite_sheet) {
    stbi_image_free(sprite_sheet->pixels);
}
