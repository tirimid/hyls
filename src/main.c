// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "gwindow.h"
#include "util.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "choreo.h"
#include "tmdf.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TICKS_PER_SECOND 100

static void exit_clean(void)
{
    SDL_Quit();
    UTIL_PRINT_LOG("exiting and cleaning");
}

int main(void)
{
    atexit(exit_clean);
    
    SDL_Init(SDL_INIT_EVERYTHING);

    struct gwindow     gwnd           = gwindow_create(800, 600, "hyls");
    struct type_vec2   gwnd_size      = {(float)gwindow_size_x(&gwnd), (float)gwindow_size_y(&gwnd)};
    struct keyboard    kbd            = keyboard_create(16);
    struct map         map            = map_create(128);
    struct enemy_group enemies        = enemy_group_create(64);
    struct proj_group  friendly_projs = proj_group_create(128);
    struct proj_group  enemy_projs    = proj_group_create(512);
    struct choreo      choreo         = choreo_create(512);

    struct player player =
    {
        .pos          = {0.0f, 0.0f},
        .rad          = 13.0f,
        .friction     = 1.3f,
        .speed_normal = 1.0f,
        .speed_slow   = 0.5f,
        
        .respawn_pos   = {400.0f, 500.0f},
        .max_inv_ticks = 90,
        .shoot_ticks   = 7,
    };

    struct enemy enemy =
    {
        .type     = ENEMY_TYPE_SNIPER,
        .rad      = 16.0f,
        .lifetime = -1,
        .pos      = {200.0f, 300.0f},
        .hp       = 3,
        
        .ti.sniper =
        {
            .bullet_rad      = 8.0f,
            .bullet_lifetime = 300,
            .bullet_speed    = 2.0f,
            .shoot_ticks     = 15,
        },
    };
    enemy_group_add_enemy(&enemies, &enemy);

    enemy = (struct enemy)
    {
        .type     = ENEMY_TYPE_NUKER,
        .rad      = 16.0f,
        .lifetime = -1,
        .pos      = {400.0f, 300.0f},
        .hp       = 3,
        
        .ti.nuker =
        {
            .bullet_rad      = 8.0f,
            .bullet_lifetime = 300,
            .bullet_speed    = 2.0f,
            .bullet_cnt      = 24,
            .shoot_ticks     = 40,
        },
    };
    enemy_group_add_enemy(&enemies, &enemy);

    UTIL_PRINT_LOG("initialization complete, running");

    bool running = true;
    while (running)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            keyboard_update(&kbd, &event);
            break;
        }

        proj_group_update(&friendly_projs);
        proj_group_update(&enemy_projs);
        enemy_group_update(&enemies, player.pos, &enemy_projs, &friendly_projs);
        player_update(&player, &map, &kbd, &enemies, &enemy_projs, &friendly_projs, gwnd_size);
        choreo_update(&choreo, &enemies, &enemy_projs, &friendly_projs, &player, &map, gwnd_size);

        gwindow_clear_color(&gwnd, TYPE_COLOR_BLACK);
        map_draw(&map, &gwnd);
        enemy_group_draw(&enemies, &gwnd);
        proj_group_draw(&enemy_projs, &gwnd);
        proj_group_draw(&friendly_projs, &gwnd);
        player_draw(&player, &gwnd);

        gwindow_present(&gwnd);
        SDL_Delay(1000 / TICKS_PER_SECOND);
    }

    choreo_destroy(&choreo);
    proj_group_destroy(&enemy_projs);
    proj_group_destroy(&friendly_projs);
    enemy_group_destroy(&enemies);
    map_destroy(&map);
    keyboard_destroy(&kbd);
    gwindow_destroy(&gwnd);

    return 0;
}
