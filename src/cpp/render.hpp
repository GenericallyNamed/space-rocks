#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using emscripten::val;

// Use thread_local when you want to retrieve & cache a global JS variable once per thread.


// // thread_local const val document;
// // val canvas;
// val ctx;

/**
 * @brief Simple test of rendering engine.
*/
void render_test();

/**
 * @brief Clear the canvas.
*/
void clear_canvas();

/**
 * @brief Draw the player, including rotation.
*/
void draw_player(float x, float y, float rotation);

/**
 * @brief Draw an asteroid.
*/
void draw_asteroid(float x, float y, float rotation);

/**
 * @brief Draw a bullet.
*/
void draw_bullet(float x, float y);

// extern GameCanvas game_canvas;