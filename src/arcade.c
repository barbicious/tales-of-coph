#include "arcade.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"
#include "item_inst.h"
#include "particles.h"
#include "state.h"

#define TILE_HEALTH 6
#define TILE_DATA_UNINITIALIZED (-1)

#define CREATE_DAMAGE_PARTICLE(n)\
    char str[3];\
    snprintf(str, sizeof(str), "%d", n);\
    particle_t* particle = particle_create(PARTICLE_TYPE_DAMAGE_TEXT, x * TILE_WIDTH, y * TILE_HEIGHT);\
    particle_set_text(particle, str);\
    list_append(&arcade->particles, particle);

void arcade_init(arcade_t* arcade) {
    /* Init Items */
    item_create(ITEM_POWER_GLOVE, TOOL_OMNI);
    item_create(ITEM_STONE, TOOL_NONE);

    /* Init Tiles */
    tile_create(TILE_GRASS);
    tile_create(TILE_WATER);
    tile_create(TILE_STONE);
    tile_create(TILE_DIRT);
    tile_create(TILE_FLOWER);
    tile_create(TILE_TREE);

    /* Generate Arcade */
    arcade->level = LEVEL_OVERWORLD;

    for (i32 y = 0; y < ARCADE_HEIGHT; y++) {
        for (i32 x = 0; x < ARCADE_WIDTH; x++) {
            f32 flower_val = perlin((f32)x / 7 + 500, (f32)y / 7 + 2500);
            f32 tree_val = perlin((f32)x / 9 + 3000, (f32)y / 9 + 1000);

            f32 val = perlin((f32)x / 8, (f32)y / 8);

            if (val > 0.2f) {
                arcade->tiles[y * ARCADE_WIDTH + x] = TILE_STONE;
            }
            else if (val > -0.2f) {
                if (tree_val > 0.3f) {
                    arcade->tiles[y * ARCADE_WIDTH + x] = TILE_TREE;
                }
                else if (flower_val > 0.2f) {
                    arcade->tiles[y * ARCADE_WIDTH + x] = TILE_FLOWER;
                }
                else {
                    arcade->tiles[y * ARCADE_WIDTH + x] = TILE_GRASS;
                }
            }
            else {
                arcade->tiles[y * ARCADE_WIDTH + x] = TILE_WATER;
            }
        }
    }

    memset(&arcade->data, TILE_DATA_UNINITIALIZED, sizeof(arcade->data));

    /* Init Pawns */
    arcade->items = list_create();
    arcade->particles = list_create();
    arcade->pawns = list_create();

    pawn_t* player = malloc(sizeof(pawn_t));
    pawn_init(player, PAWN_PLAYER, 16, 16, 0);
    arcade->pawn = player;
    list_append(&arcade->pawns, player);

    pawn_t* chest = malloc(sizeof(pawn_t));
    pawn_init(chest, PAWN_CHEST, 32, 32, 1);
    list_append(&arcade->pawns, chest);
}

void arcade_blit(arcade_t* arcade, state_t* state) {
    for (usize y = 0; y < ARCADE_HEIGHT; y++) {
        for (usize x = 0; x < ARCADE_WIDTH; x++) {
            tile_blit(arcade->tiles[y * ARCADE_WIDTH + x], state, x, y);
        }
    }

    for (usize i = 0; i < arcade->pawns.length; i++) {
        pawn_blit(list_get(&arcade->pawns, i), state);
    }

    for (usize i = 0; i < arcade->items.length; i++) {
        item_inst_blit(list_get(&arcade->items, i), state);
    }

    for (usize i = 0; i < arcade->particles.length; i++) {
        particle_blit(list_get(&arcade->particles, i), state);
    }
}

void arcade_tick(arcade_t* arcade, state_t* state) {
    for (usize i = 0; i < arcade->items.length; i++) {
        item_inst_tick(list_get(&arcade->items, i), state);
    }

    for (usize i = 0; i < arcade->particles.length; i++) {
        particle_t* particle = list_get(&arcade->particles, i);

        particle_tick(particle, state);
        if (particle->lifetime <= 0) {
            list_remove(&arcade->particles, i);
        }
    }

    for (usize i = 0; i < arcade->pawns.length; i++) {
        pawn_tick(list_get(&arcade->pawns, i), state);
    }
}

tile_type_e arcade_get_tile_at(const arcade_t* arcade, const i32 x, const i32 y) {
    if (x < 0 || x >= ARCADE_WIDTH || y < 0 || y >= ARCADE_HEIGHT) {
        return TILE_WATER;
    }

    return arcade->tiles[y * ARCADE_WIDTH + x];
}

bool arcade_tile_collides(const arcade_t* arcade, i32 x, i32 y) {
    return arcade_get_tile_at(arcade, x, y) == TILE_TREE || arcade_get_tile_at(arcade, x, y) == TILE_STONE;
}

bool arcade_attempt_hit_tile(arcade_t* arcade, const i32 x, const i32 y, item_t* item) {
    if (item->tool == TOOL_NONE) {
        return false;
    }

    tile_type_e tile = arcade_get_tile_at(arcade, x, y);

    if (tile != TILE_STONE && tile != TILE_GRASS) {
        return false;
    }

    i32 dmg = 1;

    i32* data = &arcade->data[y * ARCADE_WIDTH + x];

    if (*data == TILE_DATA_UNINITIALIZED) {
        *data = TILE_HEALTH;

        *data -= dmg;
    } else if (*data > 1) {
        *data -= dmg;
    } else {
        switch (tile) {
        case TILE_WATER: {
        }
        break;
        case TILE_GRASS: {
        }
        break;
        case TILE_DIRT: {
        }
        break;
        case TILE_STONE: {
            list_append(&arcade->items, item_inst_create(ITEM_STONE, x * TILE_WIDTH, y * TILE_HEIGHT));
        }
        break;
        }

        arcade->tiles[y * ARCADE_WIDTH + x] = TILE_DIRT;
    }

    CREATE_DAMAGE_PARTICLE(dmg);

    switch (tile) {
    case TILE_WATER: {
    } break;
    case TILE_GRASS: {
    } break;
    case TILE_DIRT: {
    } break;
    case TILE_STONE: {
        for (i32 i = 0; i < RAND_RANG(1, 4); i++) {
            particle_t* stone_particle = particle_create(PARTICLE_TYPE_STONE, x * TILE_WIDTH, y * TILE_HEIGHT);
            list_append(&arcade->particles, stone_particle);
        }
    } break;
    }

    return true;
}

pawn_t* arcade_pawn_collides(arcade_t* arcade, i32 x, i32 y, u32 id) {
    for (usize i = 0; i < arcade->pawns.length; i++) {
        if (i == id) continue;

        pawn_t* pawn = list_get(&arcade->pawns, i);

        if (RECT_COLLIDES(x, y, PAWN_WIDTH, PAWN_HEIGHT, pawn->x, pawn->y, PAWN_WIDTH, PAWN_HEIGHT)) {
            return pawn;
        }
    }

    return NULL;
}

pawn_t* arcade_pawn_collides_axis(arcade_t* arcade, i32 x, i32 y, u32 id, bool horizontal) {
    if (horizontal) {
        for (usize i = 0; i < arcade->pawns.length; i++) {
            if (i == id) continue;

            pawn_t* pawn = list_get(&arcade->pawns, i);

            if (RECT_COLLIDES(x, y, PAWN_WIDTH, PAWN_HEIGHT, pawn->x - pawn->dx, pawn->y, PAWN_WIDTH, PAWN_HEIGHT)) {
                return pawn;
            }
        }
    } else {
        for (usize i = 0; i < arcade->pawns.length; i++) {
            if (i == id) continue;

            pawn_t* pawn = list_get(&arcade->pawns, i);

            if (RECT_COLLIDES(x, y, PAWN_WIDTH, PAWN_HEIGHT, pawn->x, pawn->y - pawn->dy, PAWN_WIDTH, PAWN_HEIGHT)) {
                return pawn;
            }
        }
    }

    return NULL;
}

void arcade_destroy(arcade_t* arcade) {
    list_destroy(&arcade->particles);
    list_destroy(&arcade->pawns);
    list_destroy(&arcade->items);
}

