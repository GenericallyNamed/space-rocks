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

    /**
     * @brief Return if a key is pressed.
    */
    EMSCRIPTEN_KEEPALIVE
    bool is_key_pressed(char key);

    /**
     * @brief Return if a key is pressed continuously.
    */
    EMSCRIPTEN_KEEPALIVE
    bool is_key_pressed_continuous(char key);

    /**
     * @brief Return if a key is exclusively pressed.
    */
    EMSCRIPTEN_KEEPALIVE
    bool is_key_exclusively_pressed(char key);

}