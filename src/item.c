#include "item.h"

#include <stdio.h>
#include <string.h>

#include "state.h"

item_t items[MAX_ITEMS];

void item_create(item_type_e type, tool_type_e tool) {
    item_t item = {0};

    item.type = type;
    item.tool = tool;

    switch (item.type) {
    case ITEM_POWER_GLOVE: {
        item.sx = 72;
        item.sy = 8;

        i32 colors[4] = {
            rgb_to_palette(2, 2, 2),
            rgb_to_palette(4, 4, 4),
            rgb_to_palette(3, 2, 1),
            rgb_to_palette(4, 3, 2),
        };
        memcpy(item.colors, colors, sizeof(colors));
    }
    break;
    case ITEM_STONE: {
        item.sx = 72;
        item.sy = 16;

        i32 colors[4] = {
            rgb_to_palette(2, 2, 2),
            rgb_to_palette(3, 3, 3),
            rgb_to_palette(4, 4, 4),
            rgb_to_palette(5, 5, 5),
        };
        memcpy(item.colors, colors, sizeof(colors));
    }
    break;
    }

    items[item.type] = item;
}

void item_blit(item_t* item, state_t* state, i32 x, i32 y) {
    sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, x, y, item->sx, item->sy, 8, 8, item->colors,
                             FLIP_NONE);
}
