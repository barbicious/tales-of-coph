#pragma once
#include "utils.h"

typedef struct state state_t;

typedef enum direction {
    NORTH, SOUTH, EAST, WEST,
} direction_e;

typedef enum pawn_type {
    PLAYER,
} pawn_type_e;


typedef struct pawn {
    pawn_type_e pawn_type;
    direction_e direction;
    i32 x, y;
    i32 dx, dy;
    i32 colors[4];
} pawn_t;

void pawn_init(pawn_t* pawn, pawn_type_e pawn_type, i32 x, i32 y);
void pawn_tick(pawn_t* pawn, state_t* state);
void pawn_blit(pawn_t* pawn, state_t* state);
void pawn_destroy(pawn_t* pawn, state_t* state);