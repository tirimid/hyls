// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "gwindow.h"

#include <stddef.h>

// does not automatically call SDL_Init()
struct gwindow gwindow_create(int wnd_width, int wnd_height, const char *wnd_title)
{
    struct gwindow gwnd;
    gwnd.wnd  = SDL_CreateWindow(wnd_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wnd_width, wnd_height, 0);
    gwnd.rend = SDL_CreateRenderer(gwnd.wnd, -1, 0);
    SDL_SetRenderDrawBlendMode(gwnd.rend, SDL_BLENDMODE_BLEND);

    return gwnd;
}

// does not automatically call SDL_Quit()
void gwindow_destroy(struct gwindow *gwnd)
{
    SDL_DestroyRenderer(gwnd->rend);
    SDL_DestroyWindow(gwnd->wnd);
}

void gwindow_draw_pixel(struct gwindow *gwnd, int x, int y, struct type_color col)
{
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);
    SDL_RenderDrawPoint(gwnd->rend, x, y);
}

void gwindow_draw_line(struct gwindow *gwnd, int x0, int y0, int x1, int y1, struct type_color col)
{
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);
    SDL_RenderDrawLine(gwnd->rend, x0, y0, x1, y1);
}

void gwindow_draw_rect(struct gwindow *gwnd, int x, int y, int width, int height, struct type_color col)
{
    SDL_Rect rect =
    {
        .x = x,
        .y = y,
        .w = width,
        .h = height,
    };
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);
    SDL_RenderDrawRect(gwnd->rend, &rect);
}

void gwindow_fill_rect(struct gwindow *gwnd, int x, int y, int width, int height, struct type_color col)
{
    SDL_Rect rect =
    {
        .x = x,
        .y = y,
        .w = width,
        .h = height,
    };
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);
    SDL_RenderFillRect(gwnd->rend, &rect);
}

void gwindow_draw_circle(struct gwindow *gwnd, int x, int y, int rad, struct type_color col)
{
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);

    for (int i = -1; i < rad * 2 + 1; ++i)
    {
        for (int j = -1; j < rad * 2 + 1; ++j)
        {
            int x_off = rad - i;
            int y_off = rad - j;
            if (x_off * x_off + y_off * y_off <= rad * rad + 1 && x_off * x_off + y_off * y_off >= rad * rad - rad * 2)
                SDL_RenderDrawPoint(gwnd->rend, x + x_off, y + y_off);
        }
    }
}

void gwindow_fill_circle(struct gwindow *gwnd, int x, int y, int rad, struct type_color col)
{
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);

    for (int i = 0; i < rad * 2; ++i)
    {
        for (int j = 0; j < rad * 2; ++j)
        {
            int x_off = rad - i;
            int y_off = rad - j;
            if (x_off * x_off + y_off * y_off <= rad * rad)
                SDL_RenderDrawPoint(gwnd->rend, x + x_off, y + y_off);
        }
    }
}

void gwindow_clear_color(struct gwindow *gwnd, struct type_color col)
{
    SDL_SetRenderDrawColor(gwnd->rend, col.r, col.g, col.b, col.a);
    SDL_RenderClear(gwnd->rend);
}

void gwindow_present(struct gwindow *gwnd)
{
    SDL_RenderPresent(gwnd->rend);
}

int gwindow_size_x(struct gwindow *gwnd)
{
    int x;
    SDL_GetWindowSize(gwnd->wnd, &x, NULL);

    return x;
}

int gwindow_size_y(struct gwindow *gwnd)
{
    int y;
    SDL_GetWindowSize(gwnd->wnd, NULL, &y);

    return y;
}

const char *gwindow_title(struct gwindow *gwnd)
{
    return SDL_GetWindowTitle(gwnd->wnd);
}
