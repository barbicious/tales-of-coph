#include "state.h"

#include "renderer.h"

#include <SDL2/SDL.h>

#define NS_PER_TICK (1.0 / 60.0)

void state_init(state_t* state, void (*tick)(struct state* state), void (*blit)(struct state* state)) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        SDL_Quit();
    }

    state->tick = tick;
    state->blit = blit;

    window_t window = {0};
    window_init(&window, string_new("Tales of Coph"), SCREEN_WIDTH * PIXEL_SCALE, SCREEN_HEIGHT * PIXEL_SCALE);
    state->window = window;

    renderer_t renderer = {0};
    renderer_init(&renderer, window.window);
    state->renderer = renderer;

    state->running = true;

    sprite_sheet_t sprite_sheet = {0};
    sprite_sheet_init(&sprite_sheet, string_new("../res/tiles.png"));
    state->sprite_sheet = sprite_sheet;

    bitmap_font_t font = {0};
    bitmap_font_init(&font);
    state->font = font;
}

void state_destroy(state_t* state) {
    bitmap_font_destroy(&state->font);
    sprite_sheet_destroy(&state->sprite_sheet);
}

void state_loop(state_t* state) {
    f64 tick_current_time = (f64)SDL_GetPerformanceCounter() / (f64)SDL_GetPerformanceFrequency();
    f64 tick_last_time = tick_current_time;

    f64 accumulator = 0;

    u32 frames = 0, ticks = 0;

    f64 frame_current_time = (f64)SDL_GetPerformanceCounter() / (f64)SDL_GetPerformanceFrequency();
    f64 frame_last_time = frame_current_time;

    while (state->running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                state->running = false;
            }
        }

        frame_current_time = (f64)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();

        if (frame_current_time - frame_last_time > 1.0) {
            frame_last_time = frame_current_time;
            printf("fps: %d | tps: %d\n", frames, ticks);
            frames = ticks = 0;
        }

        tick_current_time = (f64)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();;
        f32 delta_time = (tick_current_time - tick_last_time);
        tick_last_time = tick_current_time;

        accumulator += delta_time;

        while (accumulator >= NS_PER_TICK) {
            accumulator -= NS_PER_TICK;
            ticks++;
            state->ticks++;

            state->tick(state);
        }

        renderer_flush(&state->renderer);

        state->blit(state);

        frames++;
        state->frames++;

        renderer_display(&state->renderer);
    }

    state_destroy(state);
}
