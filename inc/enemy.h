// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef ENEMY_H__
#define ENEMY_H__

#include "type.h"
#include "gwindow.h"
#include "proj.h"

enum enemy_type
{
    ENEMY_TYPE_SNIPER,
    ENEMY_TYPE_NUKER,
    ENEMY_TYPE_SPLITTER,
};

struct enemy
{
    enum enemy_type type;
    
    struct type_vec2 pos;
    struct type_vec2 vel;
    float rad;

    int lifetime;
    int pv_lifetime_cntr;
    int hp;

    union
    {
        struct
        {
            float bullet_speed;
            float bullet_rad;
            int   bullet_lifetime;
            int   shoot_ticks;
            int   pv_shoot_tick_cntr;
        } sniper;

        struct
        {
            float bullet_speed;
            float bullet_rad;
            int   bullet_lifetime;
            int   bullet_cnt;
            int   shoot_ticks;
            int   pv_shoot_tick_cntr;
        } nuker;

        struct
        {
            float bullet_speed;
            float bullet_rad;
            int   bullet_lifetime;
            int   bullet_cnt;
            int   max_splits;
            int   shoot_ticks;
            int   pv_shoot_tick_cntr;
        } splitter;
    } ti;
};

struct enemy_group
{
    struct enemy *enemies;
    int enemy_cnt;
	int pv_max_enemy_cnt;
};

struct enemy_group enemy_group_create(int max_enemy_cnt);
void enemy_group_destroy(struct enemy_group *enemy_group);

void enemy_group_add_enemy(struct enemy_group *enemy_group, const struct enemy *enemy);
void enemy_group_remove_enemy(struct enemy_group *enemy_group, struct enemy *enemy);

void enemy_group_update(struct enemy_group *enemy_group, struct type_vec2 player_pos, struct proj_group *enemy_projs,
                        struct proj_group *friendly_projs);
void enemy_group_draw(const struct enemy_group *enemy_group, struct gwindow *gwnd);

#endif
