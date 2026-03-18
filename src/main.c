#include <stdio.h>

#define UTILS_IMPL
#include <SDL2/SDL_scancode.h>

#include "hud.h"
#include "utils.h"

#include "state.h"
#include "item.h"

void tick(state_t* state) {
    arcade_tick(&state->arcade, state);
}

void blit(state_t* state) {
    arcade_blit(&state->arcade, state);
    bitmap_font_blit_str(&state->font, &state->sprite_sheet, &state->renderer, string_create("HELLO"), 0, 40,
                         rgb_to_palette(2, 2, 2), OPAQUE, FONT_FANCY);

    blit_hud(&state->renderer, &state->sprite_sheet, &state->arcade.pawn);
}

/* PROPIERTYARY JINGOBIT MAIN!!!! @author barbicious GNU 67 LICENSCE!!!! */

i32 main(void) {
    state_t state = {0};

    state_init(&state, tick, blit);

    /* List testbed */
    list_t list = list_create();
    list_append(&list, &items[ITEM_STONE]);
    list_append(&list, &items[ITEM_POWER_GLOVE]);
    list_append(&list, &items[ITEM_STONE]);
    list_append(&list, &items[ITEM_STONE]);
    printf("%d\n", ((item_t*)list.items[2])->sy);
    list_destroy(&list);
    state_loop(&state);

    return 0;
}

/* jk lmao */
