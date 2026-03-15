//
// Created by barbicious on 3/14/26.
//

#include "window.h"

#include <SDL2/SDL.h>

void window_init(window_t *window, const char *title, i32 width, i32 height) {
    window->window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
}