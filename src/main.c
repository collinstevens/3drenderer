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

global_variable u32 window_width = 800;
global_variable u32 window_height = 600;

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

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
    for (size_t y = 0; y < window_height; y++) {
        for (size_t x = 0; x < window_width; x++) {
            color_buffer[window_width * y + x] = color;
        }
    }
    
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_color_buffer();
    clear_color_buffer(0xFFFFFF00);

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