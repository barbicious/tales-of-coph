#pragma once
#include "utils.h"

typedef struct state state_t;

typedef enum particle_type {
    PARTICLE_TYPE_STONE = 0,
    PARTICLE_TYPE_DAMAGE_TEXT,
} particle_type_e;

typedef struct particle {
    i32 x, y;
    i32 vx, vy;
    i32 lifetime;
    i32 bounces;
    particle_type_e type;
    string_t text;
} particle_t;

particle_t* particle_create(particle_type_e type, i32 x, i32 y);
void particle_set_text(particle_t* particle, const char* text);
void particle_blit(particle_t* particle, state_t* state);
void particle_tick(particle_t* particle, state_t* state);
void particle_destroy(particle_t* particle);