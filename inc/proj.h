#ifndef PROJ_H__
#define PROJ_H__

#include "type.h"
#include "gwindow.h"

#include <stdbool.h>

struct proj
{
    int lifetime;
    int pv_lifetime_cntr;
    
    struct type_vec2 pos;
    struct type_vec2 vel;
    float rad;
};

/* an individual `proj_group` should be created for friendly and enemy projectiles
 * this is a system which should be faster than simply having a `bool friendly` field
 */
struct proj_group
{
    struct proj *projs;
    int proj_cnt;
    int pv_max_proj_cnt;
};

struct proj_group proj_group_create(int max_proj_cnt);
void proj_group_destroy(struct proj_group *proj_group);

void proj_group_update(struct proj_group *proj_group);
void proj_group_draw(const struct proj_group *proj_group, struct gwindow *gwnd);

void proj_group_add_proj(struct proj_group *proj_group, const struct proj *proj);
void proj_group_remove_proj(struct proj_group *proj_group, struct proj *proj);

#endif
