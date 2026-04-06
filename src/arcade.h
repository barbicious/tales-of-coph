#pragma once

#define ARCADE_WIDTH 24
#define ARCADE_HEIGHT 24

#include "pawn.h"
#include "utils.h"
#include "tile.h"

typedef struct item item_t;
typedef struct pawn pawn_t;

typedef enum arcade_level {
    LEVEL_OVERWORLD,
} arcade_level_e;

typedef struct arcade {
    arcade_level_e level;

    pawn_t* pawn;

    tile_type_e tiles[ARCADE_WIDTH * ARCADE_HEIGHT];
    i32 data[ARCADE_WIDTH * ARCADE_HEIGHT];

    list_t items;
    list_t particles;
    list_t pawns;
} arcade_t;

void arcade_init(arcade_t* arcade);
void arcade_blit(arcade_t* arcade, state_t* state);
void arcade_tick(arcade_t* arcade, state_t* state);
tile_type_e arcade_get_tile_at(const arcade_t* arcade, i32 x, i32 y);
bool arcade_tile_collides(const arcade_t* arcade, i32 x, i32 y);
bool arcade_attempt_hit_tile(arcade_t* arcade, i32 x, i32 y, item_t* item);
pawn_t* arcade_pawn_collides(arcade_t* arcade, i32 x, i32 y, u32 id);
pawn_t* arcade_pawn_collides_axis(arcade_t* arcade, i32 x, i32 y, u32 id, bool horizontal);
void arcade_destroy(arcade_t* arcade);
