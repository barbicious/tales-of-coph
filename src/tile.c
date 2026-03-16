#include "tile.h"

#include <string.h>

#include "state.h"

#pragma region tile_blit_fns

/* Function pointers for drawing, neighbor code is fairly ugly currently, may refactor to some kind of loop */

#define MUL_TILE_W(n) ((n * TILE_WIDTH) - state->camera.tx)
#define MUL_TILE_H(n) ((n * TILE_HEIGHT) - state->camera.ty)

void ground_tile_blit(tile_t* tile, state_t* state, i32 x, i32 y) {
    bool u = tiles[arcade_get_tile_at(&state->arcade, x, y - 1)].tile_blit == tile->tile_blit;
    bool d = tiles[arcade_get_tile_at(&state->arcade, x, y + 1)].tile_blit == tile->tile_blit;
    bool l = tiles[arcade_get_tile_at(&state->arcade, x - 1, y)].tile_blit == tile->tile_blit;
    bool r = tiles[arcade_get_tile_at(&state->arcade, x + 1, y)].tile_blit == tile->tile_blit;

    if (u && l) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 24 + ((x + 1) & 1) * TILE_SPRITE_WIDTH, ((y - 3) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (u) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 0, 8, 8, 8, tile->colors, FLIP_NONE);
        } else if (l) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 8, 0, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 0, 0, 8, 8, tile->colors, FLIP_NONE);
        }
    }

    if (u && r) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 24 + ((x + 1) & 1) * TILE_SPRITE_WIDTH, ((y - 3) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (u) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 16, 8, 8, 8, tile->colors, FLIP_NONE);
        } else if (r) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 8, 0, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 16, 0, 8, 8, tile->colors, FLIP_NONE);
        }
    }

    if (d && l) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 24 + ((x + 1) & 1) * TILE_SPRITE_WIDTH, ((y - 3) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (d) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 0, 8, 8, 8, tile->colors, FLIP_NONE);
        } else if (l) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 8, 16, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 0, 16, 8, 8, tile->colors, FLIP_NONE);
        }
    }

    if (d && r) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 24 + ((x + 1) & 1) * TILE_SPRITE_WIDTH, ((y - 3) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (d) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 16, 8, 8, 8, tile->colors, FLIP_NONE);
        } else if (r) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 8, 16, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 16, 16, 8, 8, tile->colors, FLIP_NONE);
        }
    }
}


void liquid_tile_blit(tile_t* tile, state_t* state, i32 x, i32 y) {
    bool u = tiles[arcade_get_tile_at(&state->arcade, x, y - 1)].tile_blit == tile->tile_blit;
    bool d = tiles[arcade_get_tile_at(&state->arcade, x, y + 1)].tile_blit == tile->tile_blit;
    bool l = tiles[arcade_get_tile_at(&state->arcade, x - 1, y)].tile_blit == tile->tile_blit;
    bool r = tiles[arcade_get_tile_at(&state->arcade, x + 1, y)].tile_blit == tile->tile_blit;

    if (u && l) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 24 + ((x + state->ticks / 15) & 1) * TILE_SPRITE_WIDTH, ((y + state->ticks / 30) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (u) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 0, 32, 8, 8, tile->colors, FLIP_NONE);
        } else if (l) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 8, 24, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y), 0, 24, 8, 8, tile->colors, FLIP_NONE);
        }
    }

    if (u && r) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 24 + ((x + state->ticks / 15) & 1) * TILE_SPRITE_WIDTH, ((y + state->ticks / 30) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (u) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 16, 32, 8, 8, tile->colors, FLIP_NONE);
        } else if (r) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 8, 24, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y), 16, 24, 8, 8, tile->colors, FLIP_NONE);
        }
    }

    if (d && l) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 24 + ((x + state->ticks / 15) & 1) * TILE_SPRITE_WIDTH, ((y + state->ticks / 30) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (d) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 0, 32, 8, 8, tile->colors, FLIP_NONE);
        } else if (l) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 8, 40, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x), MUL_TILE_H(y) + 8, 0, 40, 8, 8, tile->colors, FLIP_NONE);
        }
    }

    if (d && r) {
        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 24 + ((x + state->ticks / 15) & 1) * TILE_SPRITE_WIDTH, ((y + state->ticks / 30) & 1) * TILE_SPRITE_HEIGHT, TILE_SPRITE_WIDTH, TILE_SPRITE_HEIGHT, tile->colors, FLIP_NONE);
    } else {
        if (d) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 16, 32, 8, 8, tile->colors, FLIP_NONE);
        } else if (r) {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 8, 40, 8, 8, tile->colors, FLIP_NONE);
        } else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, MUL_TILE_W(x) + 8, MUL_TILE_H(y) + 8, 16, 40, 8, 8, tile->colors, FLIP_NONE);
        }
    }
}

#pragma endregion tile_blit_fns

tile_t tile_init(tile_type_e tile_type) {
    tile_t tile = {0};
    tile.tile_type = tile_type;

    switch (tile_type) {
        case TILE_GRASS: {
            i32 colors[4] = {
                rgb_to_palette(4, 3, 2),
                rgb_to_palette(1, 2, 1),
                rgb_to_palette(1, 3, 1),
                rgb_to_palette(2, 4, 2),
            };
            memcpy(tile.colors, colors, sizeof(colors));

            tile.blit = ground_tile_blit;
            tile.tile_blit = TILE_BLIT_GROUND;
        } break;
        case TILE_WATER: {
            i32 colors[4] = {
                rgb_to_palette(4, 3, 2),
                rgb_to_palette(3, 2, 1),
                rgb_to_palette(1, 1, 4),
                rgb_to_palette(2, 2, 5),
            };
            memcpy(tile.colors, colors, sizeof(colors));

            tile.blit = liquid_tile_blit;
            tile.tile_blit = TILE_BLIT_LIQUID;
        } break;
    }

    tiles[tile_type] = tile;

    return tile;
}

void tile_blit(tile_type_e tile_type, state_t* state, i32 x, i32 y) {
    tile_t tile = tiles[tile_type];
    tile.blit(&tile, state, x, y);
}