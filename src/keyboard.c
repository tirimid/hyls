#include "keyboard.h"

#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

struct keyboard keyboard_create(int max_axis_cnt)
{
    struct keyboard kbd =
    {
        .axis_cnt        = 0,
        .axes            = malloc(sizeof(struct keyboard_axis) * max_axis_cnt),
        .pv_max_axis_cnt = max_axis_cnt,
        
        .pv_key_states = malloc(sizeof(bool) * 0x21b),
    };
    memset(kbd.pv_key_states, '\0', sizeof(bool) * 0x21b);

    struct keyboard_axis axis =
    {
        .id      = KEYBOARD_AXIS_ID_HORIZONTAL,
        .pos_key = SDLK_RIGHT,
        .neg_key = SDLK_LEFT,
    };
    keyboard_add_axis(&kbd, axis);

    axis = (struct keyboard_axis)
    {
        .id      = KEYBOARD_AXIS_ID_VERTICAL,
        .pos_key = SDLK_DOWN,
        .neg_key = SDLK_UP,
    };
    keyboard_add_axis(&kbd, axis);

    return kbd;
}

void keyboard_destroy(struct keyboard *kbd)
{
    free(kbd->axes);
    free(kbd->pv_key_states);
}

void keyboard_set_key_state(struct keyboard *kbd, SDL_Keycode key, bool state)
{
    if ((key & 1 << 30) > 0)
    {
        key &= ~(1 << 30);
        key += 0x100;
    }
    
    kbd->pv_key_states[key] = state;
}

bool keyboard_key_pressed(const struct keyboard *kbd, SDL_Keycode key)
{
    if ((key & 1 << 30) > 0)
    {
        key &= ~(1 << 30);
        key += 0x100;
    }

    return kbd->pv_key_states[key];
}

/* easier handler for events */
void keyboard_update(struct keyboard *kbd, const SDL_Event *key_event)
{
    SDL_Keycode keycode = SDL_GetKeyFromScancode(key_event->key.keysym.scancode);
    
    switch (key_event->type)
    {
    case SDL_KEYDOWN:
        keyboard_set_key_state(kbd, keycode, true);
        break;
    case SDL_KEYUP:
        keyboard_set_key_state(kbd, keycode, false);
        break;
    }
}

void keyboard_add_axis(struct keyboard *kbd, struct keyboard_axis axis)
{
    if (kbd->axis_cnt >= kbd->pv_max_axis_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d key axes", kbd->pv_max_axis_cnt);
    
    for (int i = 0; i < kbd->axis_cnt; ++i)
    {
        if (kbd->axes[i].id == axis.id)
            UTIL_PRINT_ERROR("cannot add key axis under existing id");
    }

    kbd->axes[kbd->axis_cnt] = axis;
    ++kbd->axis_cnt;
}

void keyboard_remove_axis_a(struct keyboard *kbd, struct keyboard_axis *axis)
{
    uintptr_t kbd_axes_start = (uintptr_t)kbd->axes;
    uintptr_t kbd_axes_end   = (uintptr_t)(kbd->axes + kbd->pv_max_axis_cnt);
    if ((uintptr_t)axis < kbd_axes_start || (uintptr_t)axis >= kbd_axes_end)
        UTIL_PRINT_ERROR("cannot remove nonexistant key axis");

    memmove(axis, axis + 1, kbd_axes_end - (uintptr_t)axis);
    --kbd->axis_cnt;
}

void keyboard_remove_axis_i(struct keyboard *kbd, enum keyboard_axis_id id)
{
    struct keyboard_axis *axis = NULL;
    for (int i = 0; i < kbd->axis_cnt; ++i)
    {
        if (kbd->axes[i].id == id)
        {
            axis = &kbd->axes[i];
            break;
        }
    }
    if (axis == NULL)
        UTIL_PRINT_ERROR("cannot remove key axis of nonexistant id");

    keyboard_remove_axis_a(kbd, axis);
}

float keyboard_axis(const struct keyboard *kbd, enum keyboard_axis_id id)
{
    const struct keyboard_axis *axis = NULL;
    for (int i = 0; i < kbd->axis_cnt; ++i)
    {
        if (kbd->axes[i].id == id)
        {
            axis = &kbd->axes[i];
            break;
        }
    }
    if (axis == NULL)
        UTIL_PRINT_ERROR("cannot get value of nonexistant key axis");

    float axis_val = 0.0f;
    if (keyboard_key_pressed(kbd, axis->pos_key))
        axis_val += 1.0f;
    if (keyboard_key_pressed(kbd, axis->neg_key))
        axis_val -= 1.0f;

    return axis_val;
}
