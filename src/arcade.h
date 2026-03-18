#pragma once

#define ARCADE_WIDTH 24
#define ARCADE_HEIGHT 24

#include "pawn.h"
#include "utils.h"
#include "tile.h"

typedef enum arcade_level {
    LEVEL_OVERWORLD,
} arcade_level_e;

typedef struct arcade {
    arcade_level_e level;

    pawn_t pawn;

    tile_type_e tiles[ARCADE_WIDTH * ARCADE_HEIGHT];
    i32 data[ARCADE_WIDTH * ARCADE_HEIGHT];

    list_t items;
    list_t particles;
} arcade_t;

void arcade_init(arcade_t* arcade);
void arcade_blit(arcade_t* arcade, state_t* state);
void arcade_tick(arcade_t* arcade, state_t* state);
tile_type_e arcade_get_tile_at(const arcade_t* arcade, i32 x, i32 y);
bool arcade_attempt_hit_tile(arcade_t* arcade, i32 x, i32 y);
void arcade_destroy(arcade_t* arcade);
