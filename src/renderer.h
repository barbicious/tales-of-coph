#pragma once

#include "utils.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 180

#define CHANNELS 6

#define PALETTE_SIZE (CHANNELS * CHANNELS * CHANNELS)

[[nodiscard]] i32 rgb_to_palette(i32 r, i32 b, i32 g);

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;

typedef struct renderer {
    struct SDL_Renderer *renderer;
    struct SDL_Texture *texture;
    u32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    u32 palette[PALETTE_SIZE];
} renderer_t;

void renderer_init(renderer_t* renderer, struct SDL_Window *window);
void renderer_blit_palette(renderer_t *renderer);
void renderer_display(renderer_t* renderer);
void renderer_flush(renderer_t *renderer);
void renderer_set_pixel(renderer_t* renderer, i32 x, i32 y, u32 palette_index);