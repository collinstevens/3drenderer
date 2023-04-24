#include "display.h"

#include <stdbool.h>
#include <stdio.h>

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void draw_dotted_grid(u32 color, u32 step) {
    for (u32 y = 0; y < window_height; y += step) {
        for (u32 x = 0; x < window_width; x += step) {
            color_buffer[window_width * y + x] = color;
        }
    }
}

void draw_ruled_grid(u32 color, u32 step) {
    for (u32 y = 0; y < window_height; y++) {
        for (u32 x = 0; x < window_width; x += step) {
            color_buffer[window_width * y + x] = color;
        }
    }

    for (u32 y = 0; y < window_height; y += step) {
        for (u32 x = 0; x < window_width; x++) {
            color_buffer[window_width * y + x] = color;
        }
    }
}

void draw_rect(u32 in_x, u32 in_y, u32 length, u32 width, u32 color) {
    for (u32 y = in_y; y < window_height && y < in_y + width; y++) {
        for (u32 x = in_x; x < window_width && x < in_x + length; x++) {
            color_buffer[window_width * y + x] = color;
        }
    }
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        window_width * sizeof(u32)
    );

    SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        NULL,
        NULL
    );
}

void clear_color_buffer(u32 color) {
    for (u32 y = 0; y < window_height; y++) {
        for (u32 x = 0; x < window_width; x++) {
            color_buffer[window_width * y + x] = color;
        }
    }
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(color_buffer);
}