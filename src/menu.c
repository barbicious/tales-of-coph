//
// Created by barbicious on 3/19/26.
//

#include "menu.h"

#include <stdio.h>

#include "font.h"
#include "item.h"
#include "item_stack.h"
#include "renderer.h"
#include "state.h"
#include "utils.h"

void blit_menu(state_t* state) {
    if ((state->menu & MENU_INVENTORY) == MENU_INVENTORY) {
        sprite_sheet_blit_text_frame(&state->sprite_sheet, &state->renderer, &state->font, 0, 0, 0, 14,
                                 rgb_to_palette(5, 3, 0), rgb_to_palette(3, 2, 1), FLIP_NONE,
                                 string_create("INVENTORY"));

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
}
