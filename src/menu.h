#pragma once

#define MENU_NONE (1 << 0)
#define MENU_INVENTORY (1 << 1)
#define MENU_CHEST (1 << 2)

typedef struct state state_t;

void blit_menu(state_t* state);
