#include "display.h"
#include "typedefs.h"

#include <stdbool.h>
#include <SDL.h>

global_variable bool is_running = false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

u32* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

u32 window_width = 0;
u32 window_height = 0;

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

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_dotted_grid(0xFF333333, 10);
    // draw_ruled_grid(0xFF333333, 10);
    
    draw_rect(100, 100, 400, 200, 0xFFAED6F1);

    draw_pixel(20, 20, 0xFF0000FF);

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
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