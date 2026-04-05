#include "pawn.h"

#include <string.h>
#include <SDL2/SDL_scancode.h>

#include "arcade.h"
#include "keyboard.h"
#include "renderer.h"
#include "tile.h"
#include "state.h"
#include "hud.h"
#include "item.h"
#include "item_inst.h"
#include "item_stack.h"
#include "menu.h"

#define MISC_SPRITE_WIDTH 16
#define MISC_SPRITE_HEIGHT 16

#define HALF_PAWN_WIDTH 8
#define HALF_PAWN_HEIGHT 8

#define SWING_COOLDOWN 25
#define STAMINA_COOLDOWN 50

#pragma region player_globals

/* Definitely not good practice, but these exist ONLY for the player */

i32 swing_ticks_left = 0;
i32 stamina_cooldown_ticks_left = 0;
bool overshot_stamina = false;

static i32 hit_tile_x = 0, hit_tile_y = 0;
static bool hit_tile = false;

#pragma endregion player_globals

void pawn_init(pawn_t* pawn, pawn_type_e pawn_type, i32 x, i32 y) {
    pawn->pawn_type = pawn_type;
    pawn->x = x;
    pawn->y = y;
    pawn->direction = DIRECTION_SOUTH;
    pawn->equipped_item = 0;

    switch (pawn->pawn_type) {
    case PAWN_PLAYER: {
        pawn->inventory = list_create();

        pawn->health = pawn->health_max = 9;
        pawn->stamina = pawn->stamina_max = 9;

        i32 colors[4] = {
            rgb_to_palette(1, 1, 1),
            rgb_to_palette(4, 4, 4),
            rgb_to_palette(3, 2, 1),
            rgb_to_palette(4, 3, 2),
        };
        memcpy(pawn->colors, colors, sizeof(colors));

        list_append(&pawn->inventory, item_stack_create(&items[ITEM_POWER_GLOVE]));
    }
    break;
    case PAWN_CHEST: {
        pawn->health = pawn->health_max = 3;

        i32 colors[4] = {
            rgb_to_palette(1, 1, 1),
            rgb_to_palette(3, 1, 0),
            rgb_to_palette(4, 2, 1),
            rgb_to_palette(5, 3, 2),
        };
        memcpy(pawn->colors, colors, sizeof(colors));
    }
    break;
    }
}

void pawn_tick(pawn_t* pawn, state_t* state) {
    pawn->tile_x = AS_TILE_X(pawn->x + 8);
    pawn->tile_y = AS_TILE_Y(pawn->y + 8);

    pawn->dx = pawn->dy = 0;

    switch (pawn->pawn_type) {
    case PAWN_PLAYER: {
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
            }
            else {
                pawn->dy = -1;
            }
        }

        if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_D) && pawn->x < (ARCADE_WIDTH - 1) * TILE_WIDTH) {
            pawn->dx = 1;
        }

        if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_A) && pawn->x > 0) {
            if (pawn->dx != 0) {
                pawn->dx = 0;
            }
            else {
                pawn->dx = -1;
            }
        }

        if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_SPACE) &&
            ((item_stack_t*)list_get(&pawn->inventory, pawn->equipped_item))->item->tool != TOOL_NONE &&
            swing_ticks_left <= 0 &&
            pawn->stamina > 0) {
            i32 dx = 0, dy = 0;

            switch (pawn->direction) {
            case DIRECTION_NORTH: {
                dx = 0;
                dy = -1;
            }
                break;
            case DIRECTION_SOUTH: {
                dx = 0;
                dy = 1;
            }
                break;
            case DIRECTION_EAST: {
                dx = -1;
                dy = 0;
            }
                break;
            case DIRECTION_WEST: {
                dx = 1;
                dy = 0;
            }
                break;
            }

            if (arcade_attempt_hit_tile(&state->arcade, pawn->tile_x + dx, pawn->tile_y + dy,
                                        pawn->inventory.items[pawn->equipped_item])) {
                hit_tile_x = pawn->tile_x + dx;
                hit_tile_y = pawn->tile_y + dy;
                hit_tile = true;
            }

            else {
                hit_tile = false;
            }

            swing_ticks_left = SWING_COOLDOWN;

            pawn->stamina--;
            }
        else if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_SPACE) && pawn->stamina == 0) {
            stamina_cooldown_ticks_left = STAMINA_COOLDOWN;
        }

        if (keyboard_key_pressed(&state->keyboard, SDL_SCANCODE_E)) {
            state->menu ^= SDL_SCANCODE_MENU;
        }

        if ((state->menu & MENU_INVENTORY) == MENU_INVENTORY) {
            if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_DOWN) && pawn->equipped_item < pawn->inventory.length -
                1) {
                pawn->equipped_item++;
                }
            else if (keyboard_key_down(&state->keyboard, SDL_SCANCODE_UP) && pawn->equipped_item > 0) {
                pawn->equipped_item--;
            }
        }

        if (stamina_cooldown_ticks_left > 0) {
            if (state->ticks % 5 == 0) {
                overshot_stamina = !overshot_stamina;
            }
        }
    } break;
    case PAWN_CHEST: {
        bool collided = false;

        i32 dx = 0, dy = 0;

        if (RECT_COLLIDES(pawn->x, pawn->y, PAWN_WIDTH, PAWN_HEIGHT, state->arcade.pawn.x - state->arcade.pawn.dx, state->arcade.pawn.y, PAWN_WIDTH, PAWN_HEIGHT)) {
            collided = true;
            state->arcade.pawn.y -= state->arcade.pawn.dy;

            dy = state->arcade.pawn.dy;

            state->camera.ty -= dy;

            if (state->arcade.pawn.y <= SCREEN_HEIGHT / 2 - HALF_PAWN_WIDTH) {
                state->camera.ty = 0;
            }
            else if (state->arcade.pawn.y >= ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT / 2 + HUD_HEIGHT - HALF_PAWN_WIDTH) {
                state->camera.ty = ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT + HUD_HEIGHT;
            }
        }

        if (RECT_COLLIDES(pawn->x, pawn->y, PAWN_WIDTH, PAWN_HEIGHT, state->arcade.pawn.x, state->arcade.pawn.y - state->arcade.pawn.dy, PAWN_WIDTH, PAWN_HEIGHT)) {
            collided = true;
            state->arcade.pawn.x -= state->arcade.pawn.dx;

            dx = state->arcade.pawn.dx;

            state->camera.tx -= dx;

            if (state->arcade.pawn.x <= SCREEN_WIDTH / 2 - HALF_PAWN_WIDTH) {
                state->camera.tx = 0;
            }
            else if (state->arcade.pawn.x >= ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH / 2 - HALF_PAWN_WIDTH) {
                state->camera.tx = ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH;
            }
        }

        if (collided) {
            pawn->dx = dx;
            pawn->dy = dy;
        }
    } break;
    }

    if (pawn->dx < 0) {
        pawn->direction = DIRECTION_EAST;
    }
    else if (pawn->dx > 0) {
        pawn->direction = DIRECTION_WEST;
    }

    if (pawn->dy < 0) {
        pawn->direction = DIRECTION_NORTH;
    }
    else if (pawn->dy > 0) {
        pawn->direction = DIRECTION_SOUTH;
    }

    pawn->x += pawn->dx;

    if (arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x), AS_TILE_Y(pawn->y)) ||
        arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x + 15), AS_TILE_Y(pawn->y)) ||
        arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x), AS_TILE_Y(pawn->y + 15)) ||
        arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x + 15), AS_TILE_Y(pawn->y + 15)) ||
        pawn->x < 0 || pawn->x > ARCADE_WIDTH * TILE_WIDTH) {
        pawn->x -= pawn->dx;
        pawn->dx = 0;
    }

    pawn->y += pawn->dy;

    if (arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x), AS_TILE_Y(pawn->y)) ||
        arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x), AS_TILE_Y(pawn->y + 15)) ||
        arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x + 15), AS_TILE_Y(pawn->y)) ||
        arcade_tile_collides(&state->arcade, AS_TILE_X(pawn->x + 15), AS_TILE_Y(pawn->y + 15)) ||
        pawn->y < 0 || pawn->y > ARCADE_HEIGHT * TILE_WIDTH) {
        pawn->y -= pawn->dy;
        pawn->dy = 0;
    }

    if (pawn->pawn_type == PAWN_PLAYER) {
        state->camera.tx += pawn->dx;

        if (pawn->x <= SCREEN_WIDTH / 2 - HALF_PAWN_WIDTH) {
            state->camera.tx = 0;
        }
        else if (pawn->x >= ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH / 2 - HALF_PAWN_WIDTH) {
            state->camera.tx = ARCADE_WIDTH * TILE_WIDTH - SCREEN_WIDTH;
        }

        state->camera.ty += pawn->dy;

        if (pawn->y <= SCREEN_HEIGHT / 2 - HALF_PAWN_WIDTH) {
            state->camera.ty = 0;
        }
        else if (pawn->y >= ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT / 2 + HUD_HEIGHT - HALF_PAWN_WIDTH) {
            state->camera.ty = ARCADE_HEIGHT * TILE_HEIGHT - SCREEN_HEIGHT + HUD_HEIGHT;
        }

        if (swing_ticks_left <= 0 && pawn->stamina < pawn->stamina_max && state->ticks % 15 == 0 &&
            stamina_cooldown_ticks_left == 0 && arcade_get_tile_at(&state->arcade, pawn->tile_x, pawn->tile_y) !=
            TILE_WATER) {
            pawn->stamina++;
        }
    }

    if (arcade_get_tile_at(&state->arcade, pawn->tile_x, pawn->tile_y) == TILE_WATER && pawn->stamina > 0 && state->
        ticks % 30 == 0) {
        pawn->stamina--;
    }

    for (usize i = 0; i < state->arcade.items.length; i++) {
        item_inst_t* a_item = state->arcade.items.items[i];

        if (RECT_COLLIDES(a_item->x, a_item->y, ITEM_INST_WIDTH, ITEM_INST_HEIGHT, pawn->x, pawn->y, PAWN_WIDTH,
                          PAWN_HEIGHT)) {
            for (usize j = 0; j < pawn->inventory.length; j++) {
                item_stack_t* p_item = pawn->inventory.items[j];

                if (p_item->item->type == a_item->item->type) {
                    p_item->amount++;
                    list_remove(&state->arcade.items, i);
                    goto handled_item;
                }
            }

            list_append(&pawn->inventory, item_stack_create(a_item->item));
            list_remove(&state->arcade.items, i);
        }
    }

handled_item:


}

void pawn_blit(pawn_t* pawn, state_t* state) {
    switch (pawn->pawn_type) {
    case PAWN_PLAYER: {
        static i32 sx = 0, sy = 128, flags = FLIP_NONE;

        if (pawn->dx == 1) {
            sx = 32 + (pawn->x >> 4 & 1) * 16;
            flags = FLIP_HORIZONTAL;
        }
        else if (pawn->dx == -1) {
            sx = 32 + (pawn->x >> 4 & 1) * 16;
            flags = FLIP_NONE;
        }

        if (pawn->dy == 1) {
            sx = 0;
            if (pawn->y >> 4 & 1) {
                flags = FLIP_NONE;
            }
            else {
                flags = FLIP_HORIZONTAL;
            }
        }
        else if (pawn->dy == -1) {
            sx = 16;
            if (pawn->y >> 4 & 1) {
                flags = FLIP_NONE;
            }
            else {
                flags = FLIP_HORIZONTAL;
            }
        }

        if (arcade_get_tile_at(&state->arcade, pawn->tile_x, pawn->tile_y) == TILE_WATER) {
            i32 swimming_colors[] = {
                OPAQUE,
                OPAQUE,
                rgb_to_palette(1, 1, 4),
                rgb_to_palette(2, 2, 5),
            };

            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx,
                                     pawn->y - state->camera.ty, 0 + (state->ticks >> 5 & 1) * 16, 112, PAWN_WIDTH,
                                     PAWN_HEIGHT, swimming_colors, flags);

            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx,
                                     pawn->y - state->camera.ty, sx, sy, PAWN_WIDTH, PAWN_HEIGHT / 2, pawn->colors,
                                     flags);
        }
        else {
            sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx,
                                     pawn->y - state->camera.ty, sx, sy, PAWN_WIDTH, PAWN_HEIGHT, pawn->colors, flags);
        }

        if (swing_ticks_left > 0) {
            i32 misc_colors[] = {
                OPAQUE,
                OPAQUE,
                OPAQUE,
                rgb_to_palette(5, 5, 5),
            };


            switch (pawn->direction) {
            case DIRECTION_NORTH: {
                sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx,
                                         pawn->y - state->camera.ty - PAWN_HEIGHT, 48, 112, MISC_SPRITE_WIDTH,
                                         MISC_SPRITE_HEIGHT, misc_colors, FLIP_NONE);
            }
            break;
            case DIRECTION_SOUTH: {
                sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx,
                                         pawn->y - state->camera.ty + PAWN_HEIGHT, 48, 112, MISC_SPRITE_WIDTH,
                                         MISC_SPRITE_HEIGHT, misc_colors, FLIP_VERTICAL);
            }
            break;
            case DIRECTION_EAST: {
                sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer,
                                         pawn->x - state->camera.tx - PAWN_WIDTH, pawn->y - state->camera.ty, 64, 112,
                                         MISC_SPRITE_WIDTH, MISC_SPRITE_HEIGHT, misc_colors, FLIP_HORIZONTAL);
            }
            break;
            case DIRECTION_WEST: {
                sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer,
                                         pawn->x - state->camera.tx + PAWN_WIDTH, pawn->y - state->camera.ty, 64, 112,
                                         MISC_SPRITE_WIDTH, MISC_SPRITE_HEIGHT, misc_colors, FLIP_NONE);
            }
            break;
            }

            if (hit_tile) {
                sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer,
                                         hit_tile_x * TILE_WIDTH - state->camera.tx,
                                         hit_tile_y * TILE_HEIGHT - state->camera.ty, 32, 112, MISC_SPRITE_WIDTH,
                                         MISC_SPRITE_HEIGHT, misc_colors, FLIP_NONE);
            }
        }
    }
    break;
    case PAWN_CHEST: {

        sprite_sheet_blit_sprite(&state->sprite_sheet, &state->renderer, pawn->x - state->camera.tx,
                         pawn->y - state->camera.ty, 88, 0, PAWN_WIDTH,
                         PAWN_HEIGHT, pawn->colors, FLIP_NONE);
    }
    }
}

void pawn_destroy(pawn_t* pawn) {
    list_destroy(&pawn->inventory);
}
