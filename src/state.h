#pragma once

#include "renderer.h"
#include "window.h"
#include "sprite_sheet.h"

typedef struct state {
    bool running;

    u32 ticks, frames;

    void (*tick)(struct state *state);
    void (*blit)(struct state *state);

    window_t window;
    sprite_sheet_t sprite_sheet;
    renderer_t renderer;
} state_t;

void state_init(state_t *state, void (*tick)(struct state *state), void (*blit)(struct state *state));
void state_loop(state_t *state);