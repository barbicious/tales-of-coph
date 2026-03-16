#include <stdio.h>

#define UTILS_IMPL
#include <SDL2/SDL_scancode.h>

#include "utils.h"

#include "state.h"
#include "tile.h"

void tick(state_t* state) {
    arcade_tick(&state->arcade, state);
}

void blit(state_t* state) {
    arcade_blit(&state->arcade, state);
    bitmap_font_blit_str(&state->font, &state->sprite_sheet, &state->renderer, string_new("HELLO"), 0, 40, rgb_to_palette(2, 2, 2), OPAQUE, FONT_FANCY);
}

/* PROPIERTYARY JINGOBIT MAIN!!!! @author barbicious GNU 67 LICENSCE!!!! */

i32 main(void) {
    state_t state = {0};

    state_init(&state, tick, blit);

    state_loop(&state);

    return 0;
}

/* jk lmao */
