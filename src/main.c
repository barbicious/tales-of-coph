#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include "state.h"

void tick(state_t* state) {

}

void blit(state_t* state) {
    i32 colors[] = {rgb_to_palette(3, 2, 1), rgb_to_palette(1, 2, 1), rgb_to_palette(1, 3, 1), rgb_to_palette(1, 4, 1)};
    sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, 0, 0, 8, 0, 24, 24, colors);
    bitmap_font_blit_str(&state->font, &state->sprite_sheet, &state->renderer, string_new("HELLO"), 0, 40, rgb_to_palette(3, 2, 1), rgb_to_palette(5, 4, 0));
}

/* PROPIERTYARY JINGOBIT MAIN!!!! */

i32 main(void) {
    state_t state = {0};

    state_init(&state, tick, blit);
    state_loop(&state);

    return 0;
}

/* jk lmao */
