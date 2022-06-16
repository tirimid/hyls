// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef CHOREO_H__
#define CHOREO_H__

#include "proj.h"
#include "enemy.h"
#include "type.h"
#include "player.h"
#include "map.h"

enum choreo_event_type
{
    CHOREO_EVENT_TYPE_SPAWN_ENEMY_PROJ,
    CHOREO_EVENT_TYPE_SPAWN_FRIENDLY_PROJ,
    CHOREO_EVENT_TYPE_SPAWN_ENEMY,
    CHOREO_EVENT_TYPE_ADD_MAP_OBJECT,
    CHOREO_EVENT_TYPE_MOVE_PLAYER,
};

struct choreo_event
{
    int tick;

    enum choreo_event_type type;
    union
    {
        struct proj       spawn_enemy_proj;
        struct proj       spawn_friendly_proj;
        struct enemy      spawn_enemy;
        struct map_object add_map_object;
        struct type_vec2  move_player;
    } ti;
};

struct choreo
{
    int elapsed_ticks;

    struct choreo_event *events;
    int event_cnt;
    int pv_max_event_cnt;
};

struct choreo choreo_create(int max_event_cnt);
void choreo_destroy(struct choreo *choreo);

void choreo_update(struct choreo *choreo, struct enemy_group *enemies, struct proj_group *enemy_projs,
                   struct proj_group *friendly_projs, struct player *player, struct map *map, struct type_vec2 wnd_size);

void choreo_add_event(struct choreo *choreo, const struct choreo_event *event); // events added late will immediately execute
void choreo_remove_event(struct choreo *choreo, struct choreo_event *event);

#endif
