#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include "state.h"

void tick(state_t *state) {

}

void blit(state_t *state) {
    sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, 0, 0, 0, 0, 24, 24);
}

i32 main(void) {
    state_t state = {0};

    state_init(&state, tick, blit);
    state_loop(&state);

    return 0;
}