#pragma once
#include "utils.h"

typedef struct item item_t;

typedef struct item_stack {
    i32 amount;
    item_t* item;
} item_stack_t;

item_stack_t* item_stack_create(item_t* item);
void item_stack_destroy(item_stack_t* stack);
