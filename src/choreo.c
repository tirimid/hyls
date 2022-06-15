#include "choreo.h"

#include <stdio.h>

struct choreo choreo_create(int max_event_cnt)
{
    return (struct choreo)
    {
        .elapsed_ticks    = 0,
        .event_cnt        = 0,
        .pv_max_event_cnt = max_event_cnt,
        .events           = malloc(sizeof(struct choreo_event) * max_event_cnt),
    };
}

void choreo_destroy(struct choreo *choreo)
{
    free(choreo->events);
}

void choreo_update(struct choreo *choreo, struct enemy_group *enemies, struct proj_group *enemy_projs,
                   struct proj_group *friendly_projs, struct player *player, struct map *map)
{
    for (int i = 0; i < choreo->event_cnt; ++i)
    {
        struct choreo_event *event = &choreo->events[i];
        if (choreo->elapsed_ticks > event->tick)
            continue;
        
        switch(event->type)
        {
        case CHOREO_EVENT_TYPE_SPAWN_ENEMY_PROJ:
            proj_group_add_proj(enemy_projs, &event->ti.spawn_enemy_proj);
            break;
        case CHOREO_EVENT_TYPE_SPAWN_FRIENDLY_PROJ:
            proj_group_add_proj(friendly_projs, &event->ti.spawn_friendly_proj);
            break;
        case CHOREO_EVENT_TYPE_SPAWN_ENEMY:
            enemy_group_add_enemy(enemies, &event->ti.spawn_enemy);
            break; 
        case CHOREO_EVENT_TYPE_ADD_MAP_OBJECT:
            map_add_object(map, &event->ti.add_map_object);
            break;
        case CHOREO_EVENT_TYPE_MOVE_PLAYER:
            player_move(player, event->ti.move_player, map);
            break;
        }
    }
    
    ++choreo->elapsed_ticks;
}

/* events added late will immediately execute */
void choreo_add_event(struct choreo *choreo, const struct choreo_event *event)
{
}

void choreo_remove_event(struct choreo *choreo, struct choreo_event *event)
{
}
