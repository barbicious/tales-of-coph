#include "window.h"

#include <SDL2/SDL.h>

void window_init(window_t* window, string_t title, i32 width, i32 height) {
    window->window = SDL_CreateWindow(title.data, 0, 0, width, height, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    if (window->window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
    }

    window->width = width;
    window->height = height;

    string_free(&title);
}
