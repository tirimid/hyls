// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "enemy.h"

#include "util.h"
#include "hm.h"

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

struct enemy_group enemy_group_create(int max_enemy_cnt)
{
    return (struct enemy_group)
    {
        .enemy_cnt        = 0,
        .pv_max_enemy_cnt = max_enemy_cnt,
        .enemies          = malloc(sizeof(struct enemy) * max_enemy_cnt),
    };
}

void enemy_group_destroy(struct enemy_group *enemy_group)
{
    free(enemy_group->enemies);
}

void enemy_group_add_enemy(struct enemy_group *enemy_group, const struct enemy *enemy)
{
    if (enemy_group->enemy_cnt >= enemy_group->pv_max_enemy_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d enemies to group", enemy_group->pv_max_enemy_cnt);

    enemy_group->enemies[enemy_group->enemy_cnt] = *enemy;
    ++enemy_group->enemy_cnt;
}

void enemy_group_remove_enemy(struct enemy_group *enemy_group, struct enemy *enemy)
{
    uintptr_t enemy_group_enemies_start = (uintptr_t)enemy_group->enemies;
    uintptr_t enemy_group_enemies_end   = (uintptr_t)(enemy_group->enemies + enemy_group->enemy_cnt);
    if ((uintptr_t)enemy < enemy_group_enemies_start || (uintptr_t)enemy >= enemy_group_enemies_end)
        UTIL_PRINT_ERROR("tried removing nonexistant enemy from group");

    memmove(enemy, enemy + 1, enemy_group_enemies_end - (uintptr_t)(enemy + 1));
    --enemy_group->enemy_cnt;
}

void enemy_group_update(struct enemy_group *enemy_group, struct type_vec2 player_pos, struct proj_group *enemy_projs,
                        struct proj_group *friendly_projs)
{
    for (int i = 0; i < enemy_group->enemy_cnt; ++i)
    {
        struct enemy *enemy = &enemy_group->enemies[i];

        ++enemy->pv_lifetime_cntr;
        if (enemy->pv_lifetime_cntr > enemy->lifetime && enemy->lifetime > 0 || enemy->hp <= 0)
        {
            enemy_group_remove_enemy(enemy_group, enemy);
            
            --i;
            continue;
        }

        // enemy damaged when hit with friendly projectile
        for (int j = 0; j < friendly_projs->proj_cnt; ++j)
        {
            struct proj *proj = &friendly_projs->projs[j];
            if (util_collision_circle(enemy->pos, enemy->rad, proj->pos, proj->rad))
            {
                --enemy->hp;
                proj_group_remove_proj(friendly_projs, proj);
                
                --j;
            }
        }

        switch (enemy->type)
        {
        case ENEMY_TYPE_SNIPER:
        {
            ++enemy->ti.sniper.pv_shoot_tick_cntr;
            if (enemy->ti.sniper.pv_shoot_tick_cntr <= enemy->ti.sniper.shoot_ticks)
                break;
            enemy->ti.sniper.pv_shoot_tick_cntr = 0;

            float       bullet_speed = enemy->ti.sniper.bullet_speed;
            struct proj bullet       =
            {
                .pos      = enemy->pos,
                .vel      = type_vec2_mul_s(type_vec2_normalize(type_vec2_sub(player_pos, enemy->pos)), bullet_speed),
                .rad      = enemy->ti.sniper.bullet_rad,
                .lifetime = enemy->ti.sniper.bullet_lifetime,
            };
            proj_group_add_proj(enemy_projs, &bullet);
            
            break;
        }
        case ENEMY_TYPE_NUKER:
        {
            ++enemy->ti.nuker.pv_shoot_tick_cntr;
            if (enemy->ti.nuker.pv_shoot_tick_cntr <= enemy->ti.nuker.shoot_ticks)
                break;
            enemy->ti.nuker.pv_shoot_tick_cntr = 0;

            struct proj bullet =
            {
                .pos      = enemy->pos,
                .rad      = enemy->ti.nuker.bullet_rad,
                .lifetime = enemy->ti.nuker.bullet_lifetime,
            };

            for (int i = 0; i < enemy->ti.nuker.bullet_cnt; ++i)
            {
                float vel_angle = (float)i * 2.0f * HM_PI_F / (float)enemy->ti.nuker.bullet_cnt;
                bullet.vel = type_vec2_mul_s((struct type_vec2){cosf(vel_angle), sinf(vel_angle)}, enemy->ti.nuker.bullet_speed);
                
                proj_group_add_proj(enemy_projs, &bullet);
            }
            
            break;
        }
        case ENEMY_TYPE_SPLITTER:
            break;
        }

        enemy->pos = type_vec2_add(enemy->pos, enemy->vel);
    }
}

void enemy_group_draw(const struct enemy_group *enemy_group, struct gwindow *gwnd)
{
    for (int i = 0; i < enemy_group->enemy_cnt; ++i)
    {
        const struct enemy *enemy = &enemy_group->enemies[i];
        gwindow_draw_circle(gwnd, enemy->pos.x, enemy->pos.y, enemy->rad, TYPE_COLOR_BLUE);
    }
}
