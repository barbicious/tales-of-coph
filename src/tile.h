#pragma once
#include "utils.h"

#define MAX_TILES 255

#define TILE_SPRITE_WIDTH 8
#define TILE_SPRITE_HEIGHT 8

#define TILE_WIDTH 16
#define TILE_HEIGHT 16


typedef struct state state_t;


typedef enum tile_blit {
    TILE_BLIT_GROUND,
    TILE_BLIT_LIQUID,
} tile_blit_e;

typedef enum tile_type {
    TILE_GRASS = 0,
    TILE_WATER,
} tile_type_e;


typedef struct tile {
    tile_type_e tile_type;
    tile_blit_e tile_blit;

    // TODO: Maybe change out for just a switch statement
    void (*blit)(struct tile* tile, state_t* state, i32 x, i32 y);
    i32 colors[4];
} tile_t;

tile_t tile_init(tile_type_e tile_type);
void tile_blit(tile_type_e tile_type, state_t* state, i32 x, i32 y);

static tile_t tiles[MAX_TILES];