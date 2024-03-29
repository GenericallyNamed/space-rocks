// Wrap input state in a single object. Game code fetches from an instantiated
// input object, and the game loop updates the input object.
#include <unordered_map>
#include <stdio.h>
#include "constants.hpp"
#include "input.hpp"

// The input state
struct KeyState {
    bool pressed = false;
    bool stale = false;
    bool locked = false;
};
KeyState input_state[256];


void print_key(char key) {
    switch (key) {
        case Key::UP:
            printf("UP");
            break;
        case Key::DOWN:
            printf("DOWN");
            break;
        case Key::LEFT:
            printf("LEFT");
            break;
        case Key::RIGHT:
            printf("RIGHT");
            break;
        case Key::SPACE:
            printf("SPACE");
            break;
        case Key::ESCAPE:
            printf("ESCAPE");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
    printf("Key: %c\n", key);
}

void key_pressed(char key) {
    // printf("Key pressed: %c\n", key);

    input_state[key].pressed = true;
}

void key_released(char key) {
    // printf("Key released: %c\n", key);
    input_state[key].pressed = false;
    input_state[key].stale = false;
    input_state[key].locked = false;
}

bool is_key_pressed(char key) {
    if(input_state[key].pressed and !input_state[key].stale) {
        input_state[key].stale = true;
        return true;
    }
    return false;
}

bool is_key_pressed_continuous(char key) {
    if(input_state[key].locked) {
        return false;
    }
    return input_state[key].pressed;
}

bool is_key_exclusively_pressed(char key) {
    bool &pressed = input_state[key].pressed;
    bool exclusively_pressed = true;
    for (int i = 0; i < 256; i++) {
        if (i != key && input_state[i].pressed) {
            exclusively_pressed = false;
        }
    }
    return pressed && exclusively_pressed;
}

bool lock_key(char key) {
    input_state[key].stale = true;
    input_state[key].locked = true;
    return true;
}