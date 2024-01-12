#include <emscripten/emscripten.h>

extern "C" {
    /**
     * @brief Invoked upon a key press.
    */
    EMSCRIPTEN_KEEPALIVE
    void key_pressed(char key);

    /**
     * @brief Invoked upon a key release.
    */
    EMSCRIPTEN_KEEPALIVE
    void key_released(char key);
}

/**
 * @brief Return if a key is pressed.
*/
bool is_key_pressed(char key);

/**
 * @brief Return if a key is pressed continuously.
*/
bool is_key_pressed_continuous(char key);

/**
 * @brief Return if a key is exclusively pressed.
*/
bool is_key_exclusively_pressed(char key);

/**
 * @brief A locked key can't be used until it's released. This is useful for
 * preventing multiple actions being registered by one key press.
 * @details For example, when a player starts the game by pressing space, the
 * player won't be able to start shooting until they release space.
*/
bool lock_key(char key);