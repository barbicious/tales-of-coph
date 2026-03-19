#include <stdio.h>

#define UTILS_IMPL
#include <SDL2/SDL_scancode.h>

#include "hud.h"
#include "utils.h"

#include "state.h"
#include "item.h"
#include "item_stack.h"

void tick(state_t* state) {
    arcade_tick(&state->arcade, state);
}

void blit(state_t* state) {
    arcade_blit(&state->arcade, state);
    blit_hud(&state->renderer, &state->sprite_sheet, &state->arcade.pawn);

    sprite_sheet_blit_text_frame(&state->sprite_sheet, &state->renderer, &state->font, 0, 0, 0, 14,
                                     rgb_to_palette(5, 3, 0), rgb_to_palette(3, 2, 1), FLIP_NONE,
                                     string_create("INVENTORY"));

    /* Inventory blitting code, semantic compression and sutff. */
    for (usize i = 0; i < state->arcade.pawn.inventory.length; i++) {
        item_stack_t* item = state->arcade.pawn.inventory.items[i];

        if (i == state->arcade.pawn.equipped_item) {
            bitmap_font_blit_char(&state->font, &state->sprite_sheet, &state->renderer, '>', 8, i * 8 + 16);
            item_blit(item->item, state, 16, i * 8 + 16);
            BLIT_NUMBER(item->amount, 24, i * 8 + 16, rgb_to_palette(5, 3, 0), rgb_to_palette(3, 2, 1));
        } else {
            item_blit(item->item, state, 8, i * 8 + 16);
            BLIT_NUMBER(item->amount, 16, i * 8 + 16, rgb_to_palette(5, 3, 0), rgb_to_palette(3, 2, 1));
        }
    }
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
