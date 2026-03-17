#include "arcade.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"
#include "state.h"

#define TILE_HEALTH 3
#define TILE_DATA_UNINITIALIZED (-1)

void arcade_init(arcade_t* arcade) {
    /* Init Items */
    item_create(ITEM_POWER_GLOVE, TOOL_OMNI);
    item_create(ITEM_STONE, TOOL_NONE);

    /* Init Tiles */
    tile_create(TILE_GRASS);
    tile_create(TILE_WATER);
    tile_create(TILE_STONE);
    tile_create(TILE_DIRT);

    /* Generate Arcade */
    arcade->level = LEVEL_OVERWORLD;

    for (i32 y = 0; y < ARCADE_HEIGHT; y++) {
        for (i32 x = 0; x < ARCADE_WIDTH; x++) {
            if (rand() > RAND_MAX / 2) {
                arcade->tiles[y * ARCADE_WIDTH + x] = TILE_STONE;
            } else {
                arcade->tiles[y * ARCADE_WIDTH + x] = TILE_GRASS;
            }
        }
    }

    memset(&arcade->data, TILE_DATA_UNINITIALIZED, sizeof(arcade->data));

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

    item_blit(&items[ITEM_STONE], state, 0, 0);
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

bool arcade_attempt_hit_tile(arcade_t* arcade, const i32 x, const i32 y) {
    if (arcade_get_tile_at(arcade, x, y) != TILE_STONE && arcade_get_tile_at(arcade, x, y) != TILE_GRASS) {
        return false;
    }

    auto data = &arcade->data[y * ARCADE_WIDTH + x];

    if (*data == TILE_DATA_UNINITIALIZED) {
        *data = TILE_HEALTH;
    } else if (*data > 1) {
        *data -= 1;
    } else {
        arcade->tiles[y * ARCADE_WIDTH + x] = TILE_DIRT;
    }

    printf("hp: %d\n", arcade->data[y * ARCADE_WIDTH + x]);
    return true;
}

