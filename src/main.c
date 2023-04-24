#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define internal static 
#define local_persist static 
#define global_variable static

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

global_variable bool is_running = false;

global_variable SDL_Window* window = NULL;
global_variable SDL_Renderer* renderer = NULL;

global_variable u32* color_buffer = NULL;
global_variable SDL_Texture* color_buffer_texture = NULL;

global_variable u32 window_width = 0;
global_variable u32 window_height = 0;

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

void setup(void) {
    color_buffer = (u32*) malloc(sizeof(u32) * window_width * window_height);

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
        {
            is_running = false;
        } break;
        case SDL_KEYDOWN: 
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
        } break;
    }
}

void update(void) {

}

void draw_grid(u32 color, u32 step) {
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

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw_grid(0xFF333333, 10);
    draw_rect(100, 100, 400, 200, 0xFFAED6F1);

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(color_buffer);
}

int main(void) {
    is_running = initialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}