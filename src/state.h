#pragma once

#include "arcade.h"
#include "renderer.h"
#include "window.h"
#include "sprite_sheet.h"
#include "font.h"
#include "keyboard.h"
#include "camera.h"

typedef struct state {
    bool running;

    u32 ticks, frames;

    u32 menu;

    void (*tick)(struct state* state);
    void (*blit)(struct state* state);

    camera_t camera;

    keyboard_t keyboard;

    // TODO: Use union for scenes
    arcade_t arcade;

    window_t window;
    bitmap_font_t font;
    sprite_sheet_t sprite_sheet;
    renderer_t renderer;
} state_t;

void state_init(state_t* state, void (*tick)(struct state* state), void (*blit)(struct state* state));
void state_loop(state_t* state);
