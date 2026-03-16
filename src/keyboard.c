#include "keyboard.h"

#include <assert.h>
#include <string.h>
#include <SDL2/SDL_keyboard.h>

void keyboard_init(keyboard_t* keyboard) {
    memcpy(keyboard->current_keys_down, SDL_GetKeyboardState(nullptr), sizeof(keyboard->current_keys_down));
    memcpy(keyboard->last_keys_down, keyboard->current_keys_down, sizeof(keyboard->current_keys_down));

    assert(sizeof(keyboard->current_keys_down) == sizeof(keyboard->current_keys_down));
}

void keyboard_tick(keyboard_t* keyboard) {
    memcpy(keyboard->current_keys_down, SDL_GetKeyboardState(nullptr), sizeof(keyboard->current_keys_down));
    memcpy(keyboard->last_keys_down, keyboard->current_keys_down, sizeof(keyboard->current_keys_down));
}

b8 keyboard_key_down(keyboard_t* keyboard, u8 scancode) {
    return keyboard->current_keys_down[scancode] && keyboard->last_keys_down[scancode];
}

b8 keyboard_key_up(keyboard_t* keyboard, u8 scancode) {
}

b8 keyboard_key_pressed(keyboard_t* keyboard, u8 scancode) {
}
