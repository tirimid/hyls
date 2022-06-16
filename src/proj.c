// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "proj.h"

#include "util.h"

#include <stdlib.h>
#include <stddef.h>

struct proj_group proj_group_create(int max_proj_cnt)
{
    return (struct proj_group)
    {
        .projs           = malloc(sizeof(struct proj) * max_proj_cnt),
        .proj_cnt        = 0,
        .pv_max_proj_cnt = max_proj_cnt,
    };
}

void proj_group_destroy(struct proj_group *proj_group)
{
    free(proj_group->projs);
}

void proj_group_update(struct proj_group *proj_group)
{
    for (int i = 0; i < proj_group->proj_cnt; ++i)
    {
        struct proj *proj = &proj_group->projs[i];
        
        ++proj->pv_lifetime_cntr;
        if (proj->pv_lifetime_cntr > proj->lifetime)
        {
            proj_group_remove_proj(proj_group, proj);
            
            --i;
            continue;
        }

        proj->pos = type_vec2_add(proj->pos, proj->vel);
    }
}

void proj_group_draw(const struct proj_group *proj_group, struct gwindow *gwnd)
{
    for (int i = 0; i < proj_group->proj_cnt; ++i)
    {
        const struct proj *proj = &proj_group->projs[i];
        gwindow_draw_circle(gwnd, (int)proj->pos.x, (int)proj->pos.y, (int)proj->rad, TYPE_COLOR_WHITE);
    }
}

void proj_group_add_proj(struct proj_group *proj_group, const struct proj *proj)
{
    if (proj_group->proj_cnt >= proj_group->pv_max_proj_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d projectiles to group", proj_group->pv_max_proj_cnt);

    proj_group->projs[proj_group->proj_cnt] = *proj;
    ++proj_group->proj_cnt;
}

void proj_group_remove_proj(struct proj_group *proj_group, struct proj *proj)
{
    uintptr_t proj_group_projs_start = (uintptr_t)proj_group->projs;
    uintptr_t proj_group_projs_end   = (uintptr_t)(proj_group->projs + proj_group->proj_cnt);
    if ((uintptr_t)proj < proj_group_projs_start || (uintptr_t)proj >= proj_group_projs_end)
        UTIL_PRINT_ERROR("tried removing nonexistant projectile");

    memmove(proj, proj + 1, proj_group_projs_end - (uintptr_t)(proj + 1));
    --proj_group->proj_cnt;
}
