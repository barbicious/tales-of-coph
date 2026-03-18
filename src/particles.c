#include "particles.h"

#include "sprite_sheet.h"
#include "state.h"

#define BASE_PARTICLE_LIFETIME 60

#define FRICTION 1

particle_t* particle_create(particle_type_e type, i32 x, i32 y) {
    particle_t* ret = malloc(sizeof(particle_t));
    ret->x = x;
    ret->y = y;
    ret->type = type;
    ret->vx = RAND_RANG(-5, 5);
    ret->vy = RAND_RANG(-3, -5);
    ret->lifetime = BASE_PARTICLE_LIFETIME;
    ret->bounces = 5;
    return ret;
}

void particle_set_text(particle_t* particle, const char* text) {
    particle->text = string_create(text);
}

void particle_blit(particle_t* particle, state_t* state) {
    if (particle->type == PARTICLE_TYPE_DAMAGE_TEXT) {
        bitmap_font_blit_str(&state->font, &state->sprite_sheet, &state->renderer, particle->text,
                             particle->x - state->camera.tx, particle->y - state->camera.ty, rgb_to_palette(5, 1, 1),
                             OPAQUE, FONT_FANCY);
    }
}

void particle_tick(particle_t* particle, state_t* state) {
    if (state->ticks % 5 != 0) {
        return;
    }

    particle->x += particle->vx;
    particle->y += particle->vy;

    i8 sign = 0;
    if (particle->vx < 0) {
        sign = -1;
    }
    else {
        sign = 1;
    }

    particle->vx -= FRICTION * sign;

    if (particle->vx < 0 && sign > 0) {
        particle->vx = 0;
    }
    else if (particle->vx > 0 && sign < 0) {
        particle->vx = 0;
    }

    particle->vy += FRICTION;

    if (particle->vy > particle->bounces) {
        particle->vy = 0;

        if (particle->bounces > 0) {
            particle->vy = (particle->bounces) * -1;
            particle->vx = 2 * RAND_RANG(-1, 1);
            particle->bounces--;
        }
    }

    if (particle->lifetime > 0) {
        particle->lifetime--;
    }
}

void particle_destroy(particle_t* particle) {
    if (particle->type == PARTICLE_TYPE_DAMAGE_TEXT) {
        particle->type = PARTICLE_TYPE_DAMAGE_TEXT;
        string_free(&particle->text);
    }
    free(particle);
}
