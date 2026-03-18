#include "hud.h"

#include <assert.h>
#include <stdio.h>

#include "pawn.h"
#include "renderer.h"
#include "sprite_sheet.h"

void blit_hud(renderer_t* renderer, sprite_sheet_t* sprite_sheet, pawn_t* pawn) {
    assert(pawn->pawn_type == PLAYER);

    renderer_blit_rect(renderer, 0, SCREEN_HEIGHT - HUD_HEIGHT, SCREEN_WIDTH, HUD_HEIGHT, rgb_to_palette(0, 0, 0));

    i32 health_colors[] = {
        rgb_to_palette(4, 0, 0),
        rgb_to_palette(5, 1, 1),
        OPAQUE,
        rgb_to_palette(5, 3, 3),
    };

    i32 dark_health_colors[] = {
        rgb_to_palette(0, 0, 0),
        rgb_to_palette(1, 1, 1),
        OPAQUE,
        rgb_to_palette(2, 2, 2),
    };

    for (i32 i = 0; i < pawn->health_max; i++) {
        if (i < pawn->health)
            sprite_sheet_blit_sprite(sprite_sheet, renderer, i * 8, SCREEN_HEIGHT - HUD_HEIGHT + 2, 72, 0, 8, 8,
                                     health_colors, FLIP_NONE);
        else
            sprite_sheet_blit_sprite(sprite_sheet, renderer, i * 8, SCREEN_HEIGHT - HUD_HEIGHT + 2, 72, 0, 8, 8,
                                     dark_health_colors, FLIP_NONE);
    }

    i32 stamina_colors[] = {
        rgb_to_palette(3, 2, 0),
        rgb_to_palette(4, 3, 1),
        rgb_to_palette(5, 4, 1),
        rgb_to_palette(5, 5, 4),
    };

    i32 dark_stamina_colors[] = {
        rgb_to_palette(3, 2, 0),
        rgb_to_palette(4, 3, 1),
        rgb_to_palette(0, 0, 0),
        rgb_to_palette(0, 0, 0),
    };

    i32 light_stamina_colors[] = {
        rgb_to_palette(4, 3, 1),
        rgb_to_palette(5, 4, 2),
        rgb_to_palette(0, 0, 0),
        rgb_to_palette(0, 0, 0),
    };

    for (i32 i = 0; i < pawn->stamina_max; i++) {
        if (overshot_stamina) {
            sprite_sheet_blit_sprite(sprite_sheet, renderer, i * 8, SCREEN_HEIGHT - HUD_HEIGHT + 10, 80, 0, 8, 8,
                                     light_stamina_colors, FLIP_NONE);
        }
        else if (i < pawn->stamina) {
            sprite_sheet_blit_sprite(sprite_sheet, renderer, i * 8, SCREEN_HEIGHT - HUD_HEIGHT + 10, 80, 0, 8, 8,
                                     stamina_colors, FLIP_NONE);
        }
        else {
            sprite_sheet_blit_sprite(sprite_sheet, renderer, i * 8, SCREEN_HEIGHT - HUD_HEIGHT + 10, 80, 0, 8, 8,
                                     dark_stamina_colors, FLIP_NONE);
        }
    }
}
