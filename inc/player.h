#ifndef PLAYER_H__
#define PLAYER_H__

#include "type.h"
#include "gwindow.h"
#include "map.h"
#include "keyboard.h"
#include "enemy.h"
#include "proj.h"

struct player
{
    struct type_vec2 pos;
    struct type_vec2 pv_vel;
    float rad;
    
    float friction;
    float speed_normal;
    float speed_slow;
    int   hp;
    int   pv_inv_tick_cntr;
    int   max_inv_ticks;

    int pv_shoot_tick_cntr;
    int shoot_ticks;
    
    struct type_vec2 respawn_pos;
};

void player_move(struct player *player, struct type_vec2 pos, const struct map *map);

void player_update(struct player *player, const struct map *map, const struct keyboard *kbd, const struct enemy_group *enemies,
                   const struct proj_group *enemy_projs, struct proj_group *friendly_projs);
void player_draw(const struct player *player, struct gwindow *gwnd);

#endif
