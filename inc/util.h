#ifndef UTIL_H__
#define UTIL_H__

#include "type.h"

#include <stdbool.h>

#define UTIL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define UTIL_PRINT_LOG(msg)         printf("log: [" __FILE__ ", L=%d]: %s\n", __LINE__, msg)
#define UTIL_PRINT_ERROR(msg)       _int_util_print_error("error: [" __FILE__ ", L=%d]: ", __LINE__, msg)
#define UTIL_PRINT_LOGF(fmt, ...)   _int_util_print_logf("log: [" __FILE__ ", L=%d]: ", __LINE__, fmt, __VA_ARGS__)
#define UTIL_PRINT_ERRORF(fmt, ...) _int_util_print_errorf("error: [" __FILE__ ", L=%d]: ", __LINE__, fmt, __VA_ARGS__)

void _int_util_print_error(const char *info, int line, const char *msg);
void _int_util_print_logf(const char *info, int line, const char *fmt, ...);
void _int_util_print_errorf(const char *info, int line, const char *fmt, ...);

bool util_collision_aabb(struct type_vec2 pos_0, struct type_vec2 size_0, struct type_vec2 pos_1, struct type_vec2 size_1);
bool util_collision_circle(struct type_vec2 pos_0, float rad_0, struct type_vec2 pos_1, float rad_1);
bool util_collision_aabb_circle(struct type_vec2 aabb_pos, struct type_vec2 aabb_size, struct type_vec2 circ_pos, float circ_rad);

int util_random_int(int lb, int ub);
float util_random_float(float lb, float ub);

#endif
