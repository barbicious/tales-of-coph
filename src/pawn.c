#include "pawn.h"

#include <string.h>
#include <SDL2/SDL_scancode.h>

#include "arcade.h"
#include "keyboard.h"
#include "renderer.h"
#include "tile.h"
#include "state.h"
#include "hud.h"

#define PAWN_WIDTH 16
#define PAWN_HEIGHT 16

#define HALF_PAWN_WIDTH 8
#define HALF_PAWN_HEIGHT 8

#define SWING_COOLDOWN 25
#define STAMINA_COOLDOWN 50

#pragma region player_globals

/* Definitely not good practice, but these exist ONLY for the player */

i32 swing_ticks_left = 0;
i32 stamina_cooldown_ticks_left = 0;
bool overshot_stamina = false;

#pragma endregion player_globals

void pawn_init(pawn_t* pawn, pawn_type_e pawn_type, i32 x, i32 y) {
    pawn->pawn_type = pawn_type;
    pawn->x = x;
    pawn->y = y;
    pawn->direction = DIRECTION_SOUTH;

    switch (pawn->pawn_type) {
        case PLAYER: {
            pawn->health = pawn->health_max = 9;
            pawn->stamina = pawn->stamina_max = 9;

            i32 colors[4] = {
                rgb_to_palette(1, 1, 1),
                rgb_to_palette(4, 4, 4),
                rgb_to_palette(3, 2, 1),
                rgb_to_palette(4, 3, 2),
            };
            memcpy(pawn->colors, colors, sizeof(colors));
        } break;
    }
}

void pawn_tick(pawn_t* pawn, state_t* state) {
    pawn->tile_x = AS_TILE_X(pawn->x + 8);
    pawn->tile_y = AS_TILE_Y(pawn->y + 8);

    pawn->dx = pawn->dy = 0;

    switch (pawn->pawn_type) {
        case PLAYER: {
            if (swing_ticks_left > 0) {
                swing_ticks_left--;
            }

            if (stamina_cooldown_ticks_left > 0) {
                stamina_cooldown_ticks_left--;

                if (stamina_cooldown_ticks_left == 0) {
                    overshot_stamina = false;
                }
            }

            if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_S) && pawn->y < (ARCADE_HEIGHT - 1) * TILE_HEIGHT) {
                pawn->dy = 1;
            }

            if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_W) && pawn->y > 0) {
                if (pawn->dy != 0) {
                    pawn->dy = 0;
                } else {
                    pawn->dy = -1;
                }
            }

            if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_D) && pawn->x < (ARCADE_WIDTH - 1) * TILE_WIDTH) {
                pawn->dx = 1;
            }

            if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_A) && pawn->x > 0) {
                if (pawn->dx != 0) {
                    pawn->dx = 0;
                } else {
                    pawn->dx = -1;
                }
            }

            if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_SPACE) && swing_ticks_left <= 0 && pawn->stamina > 0) {
                i32 dx = 0, dy = 0;

                switch (pawn->direction) {
                    case DIRECTION_NORTH: {
                        dx = 0;
                        dy = -1;
                    } break;
                    case DIRECTION_SOUTH: {
                        dx = 0;
                        dy = 1;
                    } break;
                    case DIRECTION_EAST: {
                        dx = -1;
                        dy = 0;
                    } break;
                    case DIRECTION_WEST: {
                        dx = 1;
                        dy = 0;
                    } break;
                }

                printf("hit: %d\n", arcade_attempt_hit_tile(&state->arcade, pawn->tile_x + dx, pawn->tile_y + dy));

                swing_ticks_left = SWING_COOLDOWN;

                pawn->stamina--;
            } else if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_SPACE) && pawn->stamina == 0) {
                stamina_cooldown_ticks_left = STAMINA_COOLDOWN;
            }

            if (stamina_cooldown_ticks_left > 0) {
                if (state->ticks % 5 == 0) {
                    overshot_stamina = !overshot_stamina;
                }
            }
        } break;
    }

    if (pawn->dx < 0) {
        pawn->direction = DIRECTION_EAST;
    } else if (pawn->dx > 0) {
        pawn->direction = DIRECTION_WEST;
    }

    if (pawn->dy < 0) {
        pawn->direction = DIRECTION_NORTH;
    } else if (pawn->dy > 0) {
        pawn->direction = DIRECTION_SOUTH;
    }

    pawn->x += pawn->dx;
    pawn->y += pawn->dy;

    if (pawn->pawn_type == PLAYER) {
        state->camera.tx += pawn->dx;

        if (pawn->x <= SCREEN_WIDTH / 2 - HALF_PAWN_WIDTH) {
            state->camera.tx = 0;
        } else if (pawn->x >= ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH / 2 - HALF_PAWN_WIDTH) {
            state->camera.tx = ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH;
        }

        state->camera.ty += pawn->dy;

        if (pawn->y <= SCREEN_HEIGHT / 2 - HALF_PAWN_WIDTH) {
            state->camera.ty = 0;
        } else if (pawn->y >= ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT / 2 + HUD_HEIGHT - HALF_PAWN_WIDTH) {
            state->camera.ty = ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT + HUD_HEIGHT;
        }

        if (swing_ticks_left <= 0 && pawn->stamina < pawn->stamina_max && state->ticks % 15 == 0 && stamina_cooldown_ticks_left == 0) {
            pawn->stamina++;
        }
    }
}

void pawn_blit(pawn_t* pawn, state_t* state) {
    switch (pawn->pawn_type) {
        case PLAYER: {
            static i32 sx = 0, sy = 128, flags = FLIP_NONE;

            if (pawn->dx == 1) {
                sx = 32 + (pawn->x >> 4 & 1) * 16;
                flags = FLIP_HORIZONTAL;
            } else if (pawn->dx == -1) {
                sx = 32 + (pawn->x >> 4 & 1) * 16;
                flags = FLIP_NONE;
            }

            if (pawn->dy == 1) {
                sx = 0;
                if (pawn->y >> 4 & 1) {
                    flags = FLIP_NONE;
                } else {
                    flags = FLIP_HORIZONTAL;
                }
            } else if (pawn->dy == -1) {
                sx = 16;
                if (pawn->y >> 4 & 1) {
                    flags = FLIP_NONE;
                } else {
                    flags = FLIP_HORIZONTAL;
                }
            }

            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx, pawn->y - state->camera.ty, sx, sy, PAWN_WIDTH, PAWN_HEIGHT, pawn->colors, flags);
        } break;
    }
}

void pawn_destroy(pawn_t* pawn, state_t* state) {

}