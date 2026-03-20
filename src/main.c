#include <stdio.h>

#define UTILS_IMPL
#include <SDL2/SDL_scancode.h>

#include "hud.h"
#include "utils.h"

#include "state.h"
#include "item.h"
#include "item_stack.h"
#include "menu.h"

void tick(state_t* state) {
    arcade_tick(&state->arcade, state);
}

void blit(state_t* state) {
    arcade_blit(&state->arcade, state);
    blit_hud(&state->renderer, &state->sprite_sheet, &state->arcade.pawn);
    blit_menu(state);
}

/* PROPIERTYARY JINGOBIT MAIN!!!! @author barbicious GNU 67 LICENSCE!!!! */

i32 main(void) {
    state_t state = {0};

    state_init(&state, tick, blit);
    state_loop(&state);

    return 0;
}

/* jk lmao */
