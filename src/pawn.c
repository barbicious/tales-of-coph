#include "pawn.h"

#include <string.h>
#include <SDL2/SDL_scancode.h>

#include "arcade.h"
#include "keyboard.h"
#include "renderer.h"
#include "tile.h"
#include "state.h"

#define PAWN_WIDTH 16
#define PAWN_HEIGHT 16

void pawn_init(pawn_t* pawn, pawn_type_e pawn_type, i32 x, i32 y) {
    pawn->pawn_type = pawn_type;
    pawn->x = x;
    pawn->y = y;

    switch (pawn->pawn_type) {
        case PLAYER: {
            i32 colors[4] = {
                rgb_to_palette(1, 1, 1),
                rgb_to_palette(2, 2, 1),
                rgb_to_palette(2, 3, 1),
                rgb_to_palette(4, 4, 4),
            };
            memcpy(pawn->colors, colors, sizeof(colors));
        } break;
    }
}

void pawn_tick(pawn_t* pawn, state_t* state) {
    pawn->dx = pawn->dy = 0;

    switch (pawn->pawn_type) {
        case PLAYER: {
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
        } break;
    }

    pawn->x += pawn->dx;
    pawn->y += pawn->dy;

    if (pawn->pawn_type == PLAYER) {
        state->camera.tx += pawn->dx;

        if (pawn->x <= SCREEN_WIDTH / 2) {
            state->camera.tx = 0;
        } else if (pawn->x >= ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH / 2) {
            state->camera.tx = ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH;
        }

        state->camera.ty += pawn->dy;

        if (pawn->y <= SCREEN_HEIGHT / 2) {
            state->camera.ty = 0;
        } else if (pawn->y >= ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT / 2) {
            state->camera.ty = ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT;
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
