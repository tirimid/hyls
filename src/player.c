#include "player.h"

#include "util.h"

#include <math.h>

void player_move(struct player *player, struct type_vec2 pos, const struct map *map, struct type_vec2 wnd_size)
{
    player->pos = pos;
    
    for (int i = 0; i < map->obj_cnt; ++i)
    {
        const struct map_object *obj = &map->objs[i];
        if (!util_collision_aabb_circle(obj->pos, obj->size, player->pos, player->rad))
            continue;

        struct type_vec2 obj_center = type_vec2_add(obj->pos, type_vec2_mul_s(obj->size, 0.5f));

        float x_depth = obj_center.x < player->pos.x
            ? obj->pos.x + obj->size.x - player->pos.x + player->rad
            : obj->pos.x - (player->pos.x + player->rad);
        
        float y_depth = obj_center.y < player->pos.y
            ? obj->pos.y + obj->size.y - player->pos.y + player->rad
            : obj->pos.y - (player->pos.y + player->rad);
        
        if (fabsf(x_depth) < fabsf(y_depth))
        {
            player->pos.x += x_depth;
            player->pv_vel.x = 0.0f;
        }
        else if (fabsf(x_depth) > fabsf(y_depth))
        {
            player->pos.y += y_depth;
            player->pv_vel.y = 0.0f;
        }
        else
        {
            player->pos    = type_vec2_add(player->pos, (struct type_vec2){x_depth, y_depth});
            player->pv_vel = (struct type_vec2){0.0f, 0.0f};
        }
    }

    struct type_vec2 border_offset = {player->rad, player->rad};
    player->pos = type_vec2_clamp(player->pos, border_offset, type_vec2_sub(wnd_size, border_offset));
}

void player_update(struct player *player, const struct map *map, const struct keyboard *kbd, const struct enemy_group *enemies,
                   const struct proj_group *enemy_projs, struct proj_group *friendly_projs, struct type_vec2 wnd_size)
{
    ++player->pv_shoot_tick_cntr;
    ++player->pv_inv_tick_cntr;
    
    if (player->pv_inv_tick_cntr <= player->max_inv_ticks / 3)
    {
        float lerp_t = 6.0f / (float)(player->max_inv_ticks - player->pv_inv_tick_cntr);
        player->pos  = type_vec2_lerp(player->pos, player->respawn_pos, lerp_t);
    }
    else
    {
        struct type_vec2 move_dir =
        {
            .x = keyboard_axis(kbd, KEYBOARD_AXIS_ID_HORIZONTAL),
            .y = keyboard_axis(kbd, KEYBOARD_AXIS_ID_VERTICAL),
        };
        move_dir = type_vec2_normalize(move_dir);

        float current_speed = keyboard_key_pressed(kbd, SDLK_LSHIFT) ? player->speed_slow : player->speed_normal;
        player->pv_vel      = type_vec2_add(player->pv_vel, type_vec2_mul_s(move_dir, current_speed));
        
        player_move(player, type_vec2_add(player->pos, player->pv_vel), map, wnd_size);
        player->pv_vel = type_vec2_mul_s(player->pv_vel, 1.0f / player->friction);

        for (int i = 0; i < enemies->enemy_cnt; ++i)
        {
            const struct enemy *enemy = &enemies->enemies[i];
            if (util_collision_circle(player->pos, player->rad, enemy->pos, enemy->rad)
                && player->pv_inv_tick_cntr > player->max_inv_ticks)
            {
                --player->hp;
                player->pv_inv_tick_cntr = 0;
                
                break;
            }
        }
        
        for (int i = 0; i < enemy_projs->proj_cnt; ++i)
        {
            const struct proj *enemy_proj = &enemy_projs->projs[i];
            if (util_collision_circle(player->pos, player->rad, enemy_proj->pos, enemy_proj->rad)
                && player->pv_inv_tick_cntr > player->max_inv_ticks)
            {
                --player->hp;
                player->pv_inv_tick_cntr = 0;
                
                break;
            }
        }

        if (keyboard_key_pressed(kbd, SDLK_z) && player->pv_shoot_tick_cntr > player->shoot_ticks)
        {
            player->pv_shoot_tick_cntr = 0;

            struct proj proj =
            {
                .pos      = player->pos,
                .vel      = {0.0f, -8.0f},
                .rad      = 8.0f,
                .lifetime = 100,
            };
            proj_group_add_proj(friendly_projs, &proj);
        }
    }
}

void player_draw(const struct player *player, struct gwindow *gwnd)
{
    gwindow_draw_circle(gwnd, (int)player->pos.x, (int)player->pos.y, (int)player->rad, TYPE_COLOR_RED);
}
