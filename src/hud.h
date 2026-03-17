#pragma once

#define HUD_HEIGHT 20


typedef struct pawn pawn_t;
typedef struct renderer renderer_t;
typedef struct sprite_sheet sprite_sheet_t;

void blit_hud(renderer_t* renderer, sprite_sheet_t* sprite_sheet, pawn_t* pawn);