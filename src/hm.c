// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#include "hm.h"

float hm_quake3_rsqrt(float n)
{
    float x2 = n * 0.5f;
    
    long i = 0x5f3759df - (*(const long *)&n >> 1);
    n = *(const float *)&i;
    n *= 1.5f - x2 * n * n;
    
    return n;
}

float hm_lerp(float a, float b, float t)
{
    return (1.0f - t) * a + t * b;
}

float hm_clamp(float min, float max, float n)
{
    if (n < min)
        return min;
    else if (n > max)
        return max;
    else
        return n;
}
