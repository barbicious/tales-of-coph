//
// Created by barbicious on 3/17/26.
//

#include "item_inst.h"

#include "state.h"

#define FRICTION 1

item_inst_t* item_inst_create(item_type_e type, i32 x, i32 y) {
    item_inst_t* ret = malloc(sizeof(item_inst_t));
    ret->x = x;
    ret->y = y;
    ret->vx = RAND_RANG(-5, 5);
    ret->vy = RAND_RANG(-3, -5);
    ret->item = &items[type];
    ret->bounces = 3;
    return ret;
}

void item_inst_blit(item_inst_t* inst, state_t* state) {
    item_blit(inst->item, state, inst->x - state->camera.tx, inst->y - state->camera.ty);
}

void item_inst_tick(item_inst_t* inst, state_t* state) {
    if (state->ticks % 5 != 0) {
        return;
    }

    inst->x += inst->vx;
    inst->y += inst->vy;

    i8 sign = 0;
    if (inst->vx < 0) {
        sign = -1;
    }
    else {
        sign = 1;
    }

    inst->vx -= FRICTION * sign;

    if (inst->vx < 0 && sign > 0) {
        inst->vx = 0;
    }
    else if (inst->vx > 0 && sign < 0) {
        inst->vx = 0;
    }

    inst->vy += FRICTION;

    if (inst->vy > inst->bounces) {
        inst->vy = 0;

        if (inst->bounces > 0) {
            inst->vy = (inst->bounces) * -1;
            inst->vx = 2 * RAND_RANG(-1, 1);
            inst->bounces--;
        }
    }
}