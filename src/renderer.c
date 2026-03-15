#include "renderer.h"

#include <string.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>

[[nodiscard]] f64 apply_luminance(f64 color, f64 luminance) {
    color += luminance;
    color *= 0.5;
    color *= (230.0 / 255.0);
    color += 10.0;
    return color;
}

[[nodiscard]] i32 color_map(i32 color) {
    if (color < 0)
        return 0;

    return (((color * 100) % 10) + ((color * 10) % 10) + (color % 10));
}

[[nodiscard]] i32 rgb_to_palette(i32 r, i32 b, i32 g) {
    if (r < 0 || g < 0 || b < 0)
        return 0;

    return color_map(r * (255.0 / 5.0)) * 36 +
        color_map(g * (255.0 / 5.0)) * 36 +
            color_map(b * (255.0 / 5.0)) * 36;
}

void renderer_init(renderer_t *renderer, SDL_Window *window) {
    memset(renderer->pixels, 0, sizeof(renderer->pixels));
    memset(renderer->palette, 0, sizeof(renderer->palette));

    renderer->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    renderer->texture = SDL_CreateTexture(renderer->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    u32 i = 0;
    for (u8 r = 0; r < CHANNELS; r++) {
        for (u8 g = 0; g < CHANNELS; g++) {
            for (u8 b = 0; b < CHANNELS; b++) {
                f64 rr = r * (255.0 / 5.0);
                f64 gg = g * (255.0 / 5.0);
                f64 bb = b * (255.0 / 5.0);

                f64 luminance = (rr * 30.0 + gg * 59.0 + bb * 11.0) / 100.0;

                rr = apply_luminance(rr, luminance);
                gg = apply_luminance(gg, luminance);
                bb = apply_luminance(bb, luminance);

                renderer->palette[i] = ((u32)rr << 16) | ((u32)gg << 8) | ((u32)bb << 0);

                i++;
            }
        }
    }
}

void renderer_flush(renderer_t *renderer) {
    memset(renderer->pixels, 0, sizeof(renderer->pixels));
    SDL_RenderClear(renderer->renderer);
}

void renderer_blit_palette(renderer_t *renderer) {
    for (i32 y = 0; y < SCREEN_HEIGHT; y++) {
        for (i32 x = 0; x < SCREEN_WIDTH; x++) {
            const u32 palette_index = (x + y) % PALETTE_SIZE;

            renderer->pixels[y * SCREEN_WIDTH + x] = renderer->palette[palette_index];
        }
    }
}

void renderer_display(renderer_t* renderer) {
    SDL_UpdateTexture(renderer->texture, nullptr, renderer->pixels, SCREEN_WIDTH * sizeof(u32));
    SDL_RenderCopy(renderer->renderer, renderer->texture, nullptr, nullptr);
    SDL_RenderPresent(renderer->renderer);
}

void renderer_set_pixel(renderer_t* renderer, i32 x, i32 y, u32 palette_index) {
    renderer->pixels[y * SCREEN_WIDTH + x] = renderer->palette[palette_index];
}