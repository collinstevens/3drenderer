#include "display.h"
#include "typedefs.h"
#include "vector.h"

#include <stdbool.h>
#include <SDL.h>

#define N_POINTS (9 * 9 * 9)
v3 cube_points[N_POINTS];
v2 projected_points[N_POINTS];

v3 camera_position = { .x = 0, .y = 0, .z = -5 };
v3 cube_rotation = { .x = 0, .y = 0, .z = 0 };

float fov_factor = 640;

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

    int point_count = 0;

    for (float x = -1; x <= 1; x += .25) {
        for (float y = -1; y <= 1; y += .25) {
            for (float z = -1; z <= 1; z += .25) {
                v3 point = { .x = x, .y = y, .z = z };
                cube_points[point_count++] = point;
            }
        }
    }
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

v2 project(v3 point) {
    v2 projection = {
        .x = point.x * fov_factor / point.z,
        .y = point.y * fov_factor / point.z
    };

    return projection;
}

void update(void) {
    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (int i = 0; i < N_POINTS; i++) {
        v3 point = cube_points[i];

        point = v3_rotate_x(point, cube_rotation.x);
        point = v3_rotate_y(point, cube_rotation.y);
        point = v3_rotate_z(point, cube_rotation.z);

        point.z -= camera_position.z;

        v2 projection = project(point);

        projected_points[i] = projection;
    }
}

void render(void) {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    draw_dotted_grid(0xFF333333, 10);
    // draw_ruled_grid(0xFF333333, 10);
    
    // draw_rect(100, 100, 400, 200, 0xFFAED6F1);

    // draw_pixel(20, 20, 0xFF0000FF);

    for (int i = 0; i < N_POINTS; i++) {
        v2 point = projected_points[i];
        draw_rect(
            point.x + window_width / 2,
            point.y + window_height / 2,
            4,
            4,
            0xFFFFFF00
        );
    }
    
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