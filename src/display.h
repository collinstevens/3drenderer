#ifndef DISPLAY_H
#define DISPLAY_H

#include "typedefs.h"

#include <stdbool.h>
#include <SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern u32* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern u32 window_width;
extern u32 window_height;

bool initialize_window(void);
void destroy_window(void);

void draw_dotted_grid(u32 color, u32 step);
void draw_ruled_grid(u32 color, u32 step);
void draw_rect(u32 in_x, u32 in_y, u32 length, u32 width, u32 color);
void draw_pixel(i32 x, i32 y, u32 color);

void render_color_buffer(void);
void clear_color_buffer(u32 color);

#endif