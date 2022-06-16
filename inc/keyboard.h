// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef KEYBOARD_H__
#define KEYBOARD_H__

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdint.h>

enum keyboard_axis_id
{
    KEYBOARD_AXIS_ID_HORIZONTAL = 0,
    KEYBOARD_AXIS_ID_VERTICAL   = 1,
};

struct keyboard_axis
{
    enum keyboard_axis_id id;
    SDL_Keycode pos_key;
    SDL_Keycode neg_key;
};

struct keyboard
{
    struct keyboard_axis *axes;
    int axis_cnt;
    int pv_max_axis_cnt;
    
    bool *pv_key_states;
};

struct keyboard keyboard_create(int max_axis_cnt);
void keyboard_destroy(struct keyboard *kbd);

void keyboard_set_key_state(struct keyboard *kbd, SDL_Keycode key, bool state);
bool keyboard_key_pressed(const struct keyboard *kbd, SDL_Keycode key);

void keyboard_update(struct keyboard *kbd, const SDL_Event *key_event); // easier handler for events

void keyboard_add_axis(struct keyboard *kbd, struct keyboard_axis axis);
void keyboard_remove_axis_a(struct keyboard *kbd, struct keyboard_axis *axis);
void keyboard_remove_axis_i(struct keyboard *kbd, enum keyboard_axis_id id);
float keyboard_axis(const struct keyboard *kbd, enum keyboard_axis_id id);

#endif
