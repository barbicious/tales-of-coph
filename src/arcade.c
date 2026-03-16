#include "arcade.h"

#include <stdlib.h>

#include "state.h"

void arcade_init(arcade_t* arcade) {
    /* Init Tiles */
    tile_init(TILE_GRASS);
    tile_init(TILE_WATER);

    /* Generate Arcade */
    arcade->level = LEVEL_OVERWORLD;

    for (i32 y = 0; y < ARCADE_HEIGHT; y++) {
        for (i32 x = 0; x < ARCADE_WIDTH; x++) {
            if (rand() > RAND_MAX / 2) {
                arcade->tiles[y * ARCADE_WIDTH + x] = TILE_WATER;
            } else {
                arcade->tiles[y * ARCADE_WIDTH + x] = TILE_GRASS;
            }
        }
    }

    /* Init Pawns */
    pawn_t player = {0};
    pawn_init(&player, PLAYER, 16, 16);
    arcade->pawn = player;
}

void arcade_blit(arcade_t* arcade, state_t* state) {
    for (i32 y = 0; y < ARCADE_HEIGHT; y++) {
        for (i32 x = 0; x < ARCADE_WIDTH; x++) {
            tile_blit(arcade->tiles[y * ARCADE_WIDTH + x], state, x, y);
        }
    }

    pawn_blit(&arcade->pawn, state);
}

void arcade_tick(arcade_t* arcade, state_t* state) {
    pawn_tick(&arcade->pawn, state);
}

tile_type_e arcade_get_tile_at(const arcade_t* arcade, const i32 x, const i32 y) {
    if (x < 0 || x >= ARCADE_WIDTH || y < 0 || y >= ARCADE_HEIGHT) {
        return TILE_WATER;
    }

    return arcade->tiles[y * ARCADE_WIDTH + x];
}