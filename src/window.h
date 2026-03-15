#pragma once

#include "utils.h"

struct SDL_Window;

typedef struct window {
    i32 width, height;
    struct SDL_Window* window;
} window_t;

void window_init(window_t* window, string_t title, i32 width, i32 height);
