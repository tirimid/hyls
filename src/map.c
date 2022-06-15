#include "map.h"

#include "util.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct map map_create(int max_obj_cnt)
{
    return (struct map)
    {
        .pv_max_obj_cnt = max_obj_cnt,
        .obj_cnt        = 0,
        .objs           = malloc(sizeof(struct map_object) * max_obj_cnt),
    };
}

void map_destroy(struct map *map)
{
    free(map->objs);
}

struct map_object *map_add_object(struct map *map, const struct map_object *obj)
{
    if (map->obj_cnt >= map->pv_max_obj_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d objects to map", map->pv_max_obj_cnt);

    map->objs[map->obj_cnt] = *obj;
    ++map->obj_cnt;
}

void map_remove_object(struct map *map, struct map_object *obj)
{
    uintptr_t map_objs_start = (uintptr_t)map->objs;
    uintptr_t map_objs_end   = (uintptr_t)(map->objs + map->obj_cnt);
    if ((uintptr_t)obj < map_objs_start || (uintptr_t)obj >= map_objs_end)
        UTIL_PRINT_ERROR("tried removing nonexistant map object");

    memmove(obj, obj + 1, map_objs_end - (uintptr_t)(obj + 1));
    --map->obj_cnt;
}

void map_scroll(struct map *map, struct type_vec2 scroll_amt)
{
    for (int i = 0; i < map->obj_cnt; ++i)
        map->objs[i].pos = type_vec2_add(map->objs[i].pos, scroll_amt);
}

void map_draw(const struct map *map, struct gwindow *gwnd)
{
    for (int i = 0; i < map->obj_cnt; ++i)
    {
        const struct map_object *map_obj = &map->objs[i];
        gwindow_draw_rect(gwnd, map_obj->pos.x, map_obj->pos.y, map_obj->size.x, map_obj->size.y, TYPE_COLOR_GREEN);
    }
}
