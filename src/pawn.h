#pragma once
#include "utils.h"

#define PAWN_WIDTH 16
#define PAWN_HEIGHT 16

extern i32 swing_ticks_left;
extern i32 stamina_cooldown_ticks_left;
extern bool overshot_stamina;

typedef struct state state_t;

typedef enum direction {
    DIRECTION_NORTH, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_WEST
} direction_e;

typedef enum pawn_type {
    PAWN_PLAYER,
    PAWN_CHEST,
} pawn_type_e;


typedef struct pawn {
    pawn_type_e pawn_type;
    direction_e direction;

    usize equipped_item;

    i32 x, y;
    i32 dx, dy;

    // Tile position (Centered)
    i32 tile_x, tile_y;

    i32 health, health_max;
    i32 stamina, stamina_max;

    i32 colors[4];

    list_t inventory;
} pawn_t;

void pawn_init(pawn_t* pawn, pawn_type_e pawn_type, i32 x, i32 y);
void pawn_tick(pawn_t* pawn, state_t* state);
void pawn_blit(pawn_t* pawn, state_t* state);
void pawn_destroy(pawn_t* pawn);
