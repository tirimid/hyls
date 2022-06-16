// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "type.h"

#include "hm.h"

#include <math.h>

struct type_vec2 type_vec2_add(struct type_vec2 a, struct type_vec2 b)
{
    return (struct type_vec2)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

// ret = a - b
struct type_vec2 type_vec2_sub(struct type_vec2 a, struct type_vec2 b)
{
    return (struct type_vec2)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

struct type_vec2 type_vec2_mul_v(struct type_vec2 a, struct type_vec2 b)
{
    return (struct type_vec2)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
    };
}

struct type_vec2 type_vec2_mul_s(struct type_vec2 a, float s)
{
    return (struct type_vec2)
    {
        .x = a.x * s,
        .y = a.y * s,
    };
}

float type_vec2_magnitude(struct type_vec2 a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

struct type_vec2 type_vec2_normalize(struct type_vec2 a)
{
    return type_vec2_mul_s(a, hm_quake3_rsqrt(a.x * a.x + a.y * a.y));
}

struct type_vec2 type_vec2_clamp(struct type_vec2 a, struct type_vec2 min, struct type_vec2 max)
{
    return (struct type_vec2)
    {
        .x = hm_clamp(min.x, max.x, a.x),
        .y = hm_clamp(min.y, max.y, a.y),
    };
}

struct type_vec2 type_vec2_lerp(struct type_vec2 a, struct type_vec2 b, float t)
{
    return (struct type_vec2)
    {
        .x = hm_lerp(a.x, b.x, t),
        .y = hm_lerp(a.y, b.y, t),
    };
}

struct type_vec3 type_vec3_add(struct type_vec3 a, struct type_vec3 b)
{
    return (struct type_vec3)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

// ret = a - b
struct type_vec3 type_vec3_sub(struct type_vec3 a, struct type_vec3 b)
{
    return (struct type_vec3)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

struct type_vec3 type_vec3_mul_v(struct type_vec3 a, struct type_vec3 b)
{
    return (struct type_vec3)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z,
    };
}

struct type_vec3 type_vec3_mul_s(struct type_vec3 a, float s)
{
    return (struct type_vec3)
    {
        .x = a.x * s,
        .y = a.y * s,
        .z = a.z * s,
    };
}

float type_vec3_magnitude(struct type_vec3 a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

struct type_vec3 type_vec3_normalize(struct type_vec3 a)
{
    return type_vec3_mul_s(a, hm_quake3_rsqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

struct type_vec3 type_vec3_clamp(struct type_vec3 a, struct type_vec3 min, struct type_vec3 max)
{
    return (struct type_vec3)
    {
        .x = hm_clamp(min.x, max.x, a.x),
        .y = hm_clamp(min.y, max.y, a.y),
        .z = hm_clamp(min.z, max.z, a.z),
    };
}

struct type_vec3 type_vec3_lerp(struct type_vec3 a, struct type_vec3 b, float t)
{
    return (struct type_vec3)
    {
        .x = hm_lerp(a.x, b.x, t),
        .y = hm_lerp(a.y, b.y, t),
        .z = hm_lerp(a.z, b.z, t),
    };
}

struct type_vec4 type_vec4_add(struct type_vec4 a, struct type_vec4 b)
{
    return (struct type_vec4)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
        .w = a.w + b.w,
    };
}

// ret = a - b
struct type_vec4 type_vec4_sub(struct type_vec4 a, struct type_vec4 b)
{
    return (struct type_vec4)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
        .w = a.w - b.w,
    };
}

struct type_vec4 type_vec4_mul_v(struct type_vec4 a, struct type_vec4 b)
{
    return (struct type_vec4)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z,
        .w = a.w * b.w,
    };
}

struct type_vec4 type_vec4_mul_s(struct type_vec4 a, float s)
{
    return (struct type_vec4)
    {
        .x = a.x * s,
        .y = a.y * s,
        .z = a.z * s,
        .w = a.w * s,
    };
}

float type_vec4_magnitude(struct type_vec4 a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

struct type_vec4 type_vec4_normalize(struct type_vec4 a)
{
    return type_vec4_mul_s(a, hm_quake3_rsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w));
}

struct type_vec4 type_vec4_clamp(struct type_vec4 a, struct type_vec4 min, struct type_vec4 max)
{
    return (struct type_vec4)
    {
        .x = hm_clamp(min.x, max.x, a.x),
        .y = hm_clamp(min.y, max.y, a.y),
        .z = hm_clamp(min.z, max.z, a.z),
        .w = hm_clamp(min.w, max.w, a.w),
    };
}

struct type_vec4 type_vec4_lerp(struct type_vec4 a, struct type_vec4 b, float t)
{
    return (struct type_vec4)
    {
        .x = hm_lerp(a.x, b.x, t),
        .y = hm_lerp(a.y, b.y, t),
        .z = hm_lerp(a.z, b.z, t),
        .w = hm_lerp(a.w, b.w, t),
    };
}
