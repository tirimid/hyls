// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef MAP_H__
#define MAP_H__

#include "type.h"
#include "gwindow.h"

struct map_object
{
    struct type_vec2 pos;
    struct type_vec2 size;
};

struct map
{
    struct map_object *objs;
    int obj_cnt;
    int pv_max_obj_cnt;
};

struct map map_create(int max_obj_cnt);
void map_destroy(struct map *map);

struct map_object *map_add_object(struct map *map, const struct map_object *obj);
void map_remove_object(struct map *map, struct map_object *obj);

void map_scroll(struct map *map, struct type_vec2 scroll_amt);
void map_draw(const struct map *map, struct gwindow *gwnd);

#endif
