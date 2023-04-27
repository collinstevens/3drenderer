#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "typedefs.h"
#include "vector.h"

#include <stdbool.h>
#include <SDL.h>

triangle triangles_to_render[N_MESH_FACES];

v3 camera_position = { .x = 0, .y = 0, .z = -5 };
v3 cube_rotation = { .x = 0, .y = 0, .z = 0 };

f32 fov_factor = 640;

bool is_running = false;
i32 previous_frame_time = 0;

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

v2 project(v3 point) {
    v2 projection = {
        .x = point.x * fov_factor / point.z,
        .y = point.y * fov_factor / point.z
    };

    return projection;
}

void update(void) {
    i32 ticks_since_last_frame = SDL_GetTicks() - previous_frame_time;
    
    bool ahead = ticks_since_last_frame < FRAME_TARGET_TIME; // 33 - 32 = 1
    bool behind = ticks_since_last_frame > FRAME_TARGET_TIME; // 33 - 34 = -1
    bool on_time = ticks_since_last_frame == FRAME_TARGET_TIME; // 33 - 33 = 0

    if (ahead) {
        i32 ticks_to_wait = FRAME_TARGET_TIME - ticks_since_last_frame;
        SDL_Delay(ticks_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (i32 i = 0; i < N_MESH_FACES; i++) {
        face mesh_face = mesh_faces[i];
        
        v3 face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];

        triangle triangle;

        for (i32 j = 0; j < 3; j++) {
            v3 vertex = face_vertices[j];
            
            vertex = v3_rotate_x(vertex, cube_rotation.x);
            vertex = v3_rotate_y(vertex, cube_rotation.y);
            vertex = v3_rotate_z(vertex, cube_rotation.z);

            vertex.z -= camera_position.z;

            v2 projection = project(vertex);

            projection.x += window_width / 2,
            projection.y += window_height / 2,

            triangle.points[j] = projection;
        }

        triangles_to_render[i] = triangle;
    }

    // for (i32 i = 0; i < N_POINTS; i++) {
    //     v3 point = cube_points[i];

    //     point = v3_rotate_x(point, cube_rotation.x);
    //     point = v3_rotate_y(point, cube_rotation.y);
    //     point = v3_rotate_z(point, cube_rotation.z);

    //     point.z -= camera_position.z;

    //     v2 projection = project(point);

    //     projected_points[i] = projection;
    // }
}

void render(void) {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    draw_dotted_grid(0xFF333333, 10);
    // draw_ruled_grid(0xFF333333, 10);
    
    // draw_rect(100, 100, 400, 200, 0xFFAED6F1);

    // draw_pixel(20, 20, 0xFF0000FF);

    for (i32 i = 0; i < N_MESH_FACES; i++) {
        triangle triangle = triangles_to_render[i];
        draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
        draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
        draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
    }
    
    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

i32 main(void) {
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