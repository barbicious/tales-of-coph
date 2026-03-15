#pragma once

#include "utils.h"

struct SDL_Window;

typedef struct window {
    struct SDL_Window *window;
} window_t;

void window_init(window_t *window, const char *title, i32 width, i32 height);