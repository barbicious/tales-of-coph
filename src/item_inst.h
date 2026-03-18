#pragma once
#include "item.h"
#include "utils.h"

#define ITEM_INST_WIDTH 8
#define ITEM_INST_HEIGHT 8

typedef struct state state_t;

typedef struct item_inst {
    i32 x, y;
    i32 vx, vy;
    i32 bounces;
    item_t* item;
} item_inst_t;

item_inst_t* item_inst_create(item_type_e type, i32 x, i32 y);
void item_inst_blit(item_inst_t* inst, state_t* state);
void item_inst_tick(item_inst_t* inst, state_t* state);
