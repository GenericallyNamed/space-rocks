#include <list>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using emscripten::val;

// Use thread_local when you want to retrieve & cache a global JS variable once per thread.


// // thread_local const val document;
// // val canvas;
// val ctx;

struct Particle
{
    Particle(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}
    ~Particle() = default;

    float x = 0;
    float y = 0;
    float dx = 0;
    float dy = 0;
    float lifetime = 30;
};

/**
 * @brief Simple test of rendering engine.
*/
void render_test();

/**
 * @brief Clear the canvas.
*/
void clear_canvas();

/**
 * @brief Reset rendering engine.
 * @details Elements like particles need to be deleted separately.
*/
void reset_render();

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

/**
 * @brief Init the stars.
*/
void init_stars();

/**
 * @brief Draw the stars.
*/
void draw_stars();

/**
 * @brief Draw a star.
*/
void draw_star(float x, float y, float luminosity);

/**
 * @brief Generate rocket exhaust particle from player
*/
void generate_rocket_exhaust(float x, float y, float rotation, float dx, float dy);

/**
 * @brief Generate rocket exhaust
*/
void render_particle(float x, float y);

/**
 * @brief Render the rocket exhaust
*/
void draw_rocket_exhaust();