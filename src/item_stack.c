//
// Created by barbicious on 3/18/26.
//

#include "item_stack.h"

#include <stdio.h>

#include "item.h"

item_stack_t* item_stack_create(item_t* item) {
    item_stack_t* item_stack = malloc(sizeof(item_stack_t));
    item_stack->item = item;
    item_stack->amount = 1;
    return item_stack;
}

void item_stack_destroy(item_stack_t* stack) {
    free(stack->item);
    free(stack);
}
