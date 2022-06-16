// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef TYPE_H__
#define TYPE_H__

#include <stdint.h>

#define TYPE_COLOR_RED   (struct type_color){255, 0,   0,   255}
#define TYPE_COLOR_GREEN (struct type_color){0,   255, 0,   255}
#define TYPE_COLOR_BLUE  (struct type_color){0,   0,   255, 255}
#define TYPE_COLOR_BLACK (struct type_color){0,   0,   0,   255}
#define TYPE_COLOR_WHITE (struct type_color){255, 255, 255, 255}

struct type_color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct type_vec2
{
    union
    {
        float raw[2];
        struct
        {
            float x;
            float y;
        };
    };
};

struct type_vec2 type_vec2_add(struct type_vec2 a, struct type_vec2 b);
struct type_vec2 type_vec2_sub(struct type_vec2 a, struct type_vec2 b); // ret = a - b
struct type_vec2 type_vec2_mul_v(struct type_vec2 a, struct type_vec2 b);
struct type_vec2 type_vec2_mul_s(struct type_vec2 a, float s);
float type_vec2_magnitude(struct type_vec2 a);
struct type_vec2 type_vec2_normalize(struct type_vec2 a);
struct type_vec2 type_vec2_clamp(struct type_vec2 a, struct type_vec2 min, struct type_vec2 max);
struct type_vec2 type_vec2_lerp(struct type_vec2 a, struct type_vec2 b, float t);

struct type_vec3
{
    union
    {
        float raw[3];
        struct
        {
            float x;
            float y;
            float z;
        };
    };
};

struct type_vec3 type_vec3_add(struct type_vec3 a, struct type_vec3 b);
struct type_vec3 type_vec3_sub(struct type_vec3 a, struct type_vec3 b); // ret = a - b
struct type_vec3 type_vec3_mul_v(struct type_vec3 a, struct type_vec3 b);
struct type_vec3 type_vec3_mul_s(struct type_vec3 a, float s);
float type_vec3_magnitude(struct type_vec3 a);
struct type_vec3 type_vec3_normalize(struct type_vec3 a);
struct type_vec3 type_vec3_clamp(struct type_vec3 a, struct type_vec3 min, struct type_vec3 max);
struct type_vec3 type_vec3_lerp(struct type_vec3 a, struct type_vec3 b, float t);

struct type_vec4
{
    union
    {
        float raw[4];
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
    };
};

struct type_vec4 type_vec4_add(struct type_vec4 a, struct type_vec4 b);
struct type_vec4 type_vec4_sub(struct type_vec4 a, struct type_vec4 b); // ret = a - b
struct type_vec4 type_vec4_mul_v(struct type_vec4 a, struct type_vec4 b);
struct type_vec4 type_vec4_mul_s(struct type_vec4 a, float s);
float type_vec4_magnitude(struct type_vec4 a);
struct type_vec4 type_vec4_normalize(struct type_vec4 a);
struct type_vec4 type_vec4_clamp(struct type_vec4 a, struct type_vec4 min, struct type_vec4 max);
struct type_vec4 type_vec4_lerp(struct type_vec4 a, struct type_vec4 b, float t);

#endif
