#pragma once

#include "utils.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 144

#define PIXEL_SCALE 5

#define CHANNELS 6

#define PALETTE_SIZE ((CHANNELS * CHANNELS * CHANNELS) + 1)

[[nodiscard]] i32 rgb_to_palette(i32 r, i32 g, i32 b);

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;

typedef struct renderer {
    struct SDL_Renderer* renderer;
    struct SDL_Texture* texture;
    u32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    u32 palette[PALETTE_SIZE];
} renderer_t;

void renderer_init(renderer_t* renderer, struct SDL_Window* window);
void renderer_blit_palette(renderer_t* renderer);
void renderer_display(renderer_t* renderer);
void renderer_flush(renderer_t* renderer);
void renderer_set_pixel(renderer_t* renderer, i32 x, i32 y, u32 palette_index);
void renderer_blit_rect(renderer_t* renderer, i32 ox, i32 oy, i32 width, i32 height, u32 palette_index);