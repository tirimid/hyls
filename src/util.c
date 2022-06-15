#include "util.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void _int_util_print_error(const char *info, int line, const char *msg)
{
    printf(info, line);
    printf("%s\n", msg);
    exit(-1);
}

void _int_util_print_logf(const char *info, int line, const char *fmt, ...)
{
    printf(info, line);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void _int_util_print_errorf(const char *info, int line, const char *fmt, ...)
{
    printf(info, line);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
    exit(-1);
}

bool util_collision_aabb(struct type_vec2 pos_0, struct type_vec2 size_0, struct type_vec2 pos_1, struct type_vec2 size_1)
{
    return pos_0.x + size_0.x > pos_1.x && pos_0.x < pos_1.x + size_1.x
        && pos_0.y + size_0.y > pos_1.y && pos_0.y < pos_1.y + size_1.y;
}

bool util_collision_circle(struct type_vec2 pos_0, float rad_0, struct type_vec2 pos_1, float rad_1)
{
    float dist_x = pos_1.x - pos_0.x;
    float dist_y = pos_1.y - pos_0.y;
    
    return sqrt(dist_x * dist_x + dist_y * dist_y) < rad_0 + rad_1;
}

bool util_collision_aabb_circle(struct type_vec2 aabb_pos, struct type_vec2 aabb_size, struct type_vec2 circ_pos, float circ_rad)
{
    struct type_vec2 aabb_half_exts = type_vec2_mul_s(aabb_size, 0.5f);
    struct type_vec2 aabb_center    = type_vec2_add(aabb_pos, aabb_half_exts);
    
    struct type_vec2 diff         = type_vec2_sub(circ_pos, aabb_center);
    struct type_vec2 diff_clamped = type_vec2_clamp(diff, type_vec2_mul_s(aabb_half_exts, -1.0f), aabb_half_exts);
    
    struct type_vec2 closest = type_vec2_add(aabb_center, diff_clamped);
    diff = type_vec2_sub(closest, circ_pos);

    return type_vec2_magnitude(diff) < circ_rad;
}

int util_random_int(int lb, int ub)
{
    return rand() % (ub - lb + 1) + lb;
}

float util_random_float(float lb, float ub)
{
    return (float)util_random_int((int)(lb * 100.0f), (int)(ub * 100.0f)) / 100.0f;
}
