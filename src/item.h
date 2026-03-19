#pragma once

#include "utils.h"

#include "tile.h"

#define MAX_ITEMS 255

typedef struct state state_t;

typedef enum item_type {
    ITEM_POWER_GLOVE = 0,
    ITEM_STONE,
} item_type_e;

typedef enum tool_type {
    TOOL_NONE = 0,
    TOOL_OMNI
} tool_type_e;

typedef struct item {
    item_type_e type;
    tool_type_e tool;

    i32 sx, sy;

    i32 colors[4];
} item_t;

void item_create(item_type_e type, tool_type_e tool);
void item_blit(item_t* item, state_t* state, i32 x, i32 y);
i32 item_get_damage(item_t* item, tile_type_e tile_type);

extern item_t items[MAX_ITEMS];
