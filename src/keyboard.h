#pragma once
#include "utils.h"

#define NUM_KEYS 512

typedef struct keyboard {
    u8 current_keys_down[NUM_KEYS];
    u8 last_keys_down[NUM_KEYS];
} keyboard_t;

void keyboard_init(keyboard_t* keyboard);
void keyboard_tick(keyboard_t* keyboard);
b8 keyboard_key_down(keyboard_t* keyboard, u8 scancode);
b8 keyboard_key_up(keyboard_t* keyboard, u8 scancode);
b8 keyboard_key_pressed(keyboard_t* keyboard, u8 scancode);