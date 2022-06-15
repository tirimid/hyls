#ifndef GWINDOW_H__
#define GWINDOW_H__

#include "type.h"

#include <SDL2/SDL.h>

struct gwindow
{
    SDL_Window   *wnd;
    SDL_Renderer *rend;
};

struct gwindow gwindow_create(int wnd_width, int wnd_height, const char *wnd_title); /* does not automatically call SDL_Init() */
void gwindow_destroy(struct gwindow *gwnd); /* does not automatically call SDL_Quit() */

void gwindow_draw_pixel(struct gwindow *gwnd, int x, int y, struct type_color col);
void gwindow_draw_line(struct gwindow *gwnd, int x0, int y0, int x1, int y1, struct type_color col);
void gwindow_draw_rect(struct gwindow *gwnd, int x, int y, int width, int height, struct type_color col);
void gwindow_fill_rect(struct gwindow *gwbd, int x, int y, int width, int height, struct type_color col);
void gwindow_draw_circle(struct gwindow *gwnd, int x, int y, int rad, struct type_color col);
void gwindow_fill_circle(struct gwindow *gwnd, int x, int y, int rad, struct type_color col);

void gwindow_clear_color(struct gwindow *gwnd, struct type_color col);
void gwindow_present(struct gwindow *gwnd);

int gwindow_size_x(struct gwindow *gwnd);
int gwindow_size_y(struct gwindow *gwnd);
const char *gwindow_title(struct gwindow *gwnd);

#endif
