#include <cmath>
#include <stdio.h>
#include <emscripten/emscripten.h>
#include "asteroids.hpp"
#include "constants.hpp"
#include "input.hpp"
#include "render.hpp"
#include "state.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_ASTEROIDS = 10;

class Key {
    public:
        // fill in the blanks
        static const char UP = 119;
        static const char DOWN = 115;
        static const char LEFT = 97;
        static const char RIGHT = 100;
        static const char SPACE = 32;
        static const char ESCAPE = 27;
};


void reset() {
    printf("Resetting game\n");
    game.player.x = Constants::ZERO_X;
    game.player.y = Constants::ZERO_Y;
    game.player.rotation = 0;
    game.player.dx = 0;
    game.player.dy = 0;
    game.score = 0;
}


void start() {
    reset();
    game.state = GameState::PLAYING;
    printf("Game started!\n");
}


void quit() {

}

void simulate_asteroids() {
    // Add asteroids if needed
    if(game.asteroids_cooldown > 0) {
        game.asteroids_cooldown--;
    }
    else {
        if(game.asteroids.size() < MAX_ASTEROIDS) {
            game.add_asteroid();
            game.asteroids_cooldown = Constants::ASTEROIDS_COOLDOWN;
        }
    }

    // Simulate movement of existing asteroids
    for(auto it = game.asteroids.begin(); it != game.asteroids.end(); it++) {
        it->x += it->dx;
        it->y += it->dy;
    }

}


void simulate_bullets() {
    for(auto it = game.bullets.begin(); it != game.bullets.end(); it++) {
        printf("Simulating a bullet\n");
        it->x += it->dx;
        it->y += it->dy;
    }
}


void play_frame() {
    bool accelerate = is_key_pressed_continuous(Key::UP);
    bool decelerate = is_key_pressed_continuous(Key::DOWN);
    bool rotate_left = is_key_pressed_continuous(Key::LEFT);
    bool rotate_right = is_key_pressed_continuous(Key::RIGHT);
    bool shooting = is_key_pressed(Key::SPACE) and game.player.shooting_cooldown == 0;


    // ======= Player movement =======
    /**
     * @brief Player acceleration/deceleration must take rotation into account.
     * @details We'll use sin and cos (std::sin & std::cos) to calculate the
     * acceleration applied to the player. Assume that a rotation of 0 means
     * the player is pointing up. A rotation of pi/2 means the player is
     * pointing down. A rotation of pi means the player is pointing left.
    */
    if(accelerate) {
        game.player.dx += 0.1 * std::sin(game.player.rotation);
        game.player.dy -= 0.1 * std::cos(game.player.rotation);
        /**
         * TODO: Add visual effects
        */
    }
    if(rotate_left && !rotate_right) {
        game.player.rotation -= 0.1;
    }
    if(rotate_right && !rotate_left) {
        game.player.rotation += 0.1;
    }
    if(game.player.shooting_cooldown > 0) {
        game.player.shooting_cooldown--;
    }
    if(shooting) {
        game.player.shooting_cooldown = Constants::SHOOTING_COOLDOWN;
        printf("Shooting!\n");
        game.add_bullet();
    }

    game.player.x += game.player.dx;
    game.player.y += game.player.dy;
    simulate_bullets();
    game.check_bullets();
    // ======= Asteroid simulation =======
    simulate_asteroids();
    game.check_asteroids();


    // Rendering elements
    draw_player(game.player.x, game.player.y, game.player.rotation);
    for(auto asteroid : game.asteroids) {
        draw_asteroid(asteroid.x, asteroid.y, asteroid.rotation);
    }
    for(auto bullet : game.bullets) {
        draw_bullet(bullet.x, bullet.y);
    }
    
}


void menu_frame() {

}


void game_over_frame() {

}


void pause_frame() {
    draw_player(game.player.x, game.player.y, game.player.rotation);
}


void init() {
    printf("Attempting to initialize game\n");
    reset();
    printf("Game is initialized!\n");
}

void get_frame() {
    clear_canvas();
    switch (game.state) {
        case GameState::MENU:
            if (is_key_pressed(Key::SPACE)) {
                start();
            }
            menu_frame();
            break;
        case GameState::PLAYING:
            if (game.player.lives == 0) {
                game.state = GameState::GAME_OVER;
            }
            if (is_key_pressed(Key::ESCAPE)) {
                game.state = GameState::PAUSED;
                printf("Pausing game!!!\n");
            }
            play_frame();
            // render_test();
            break;
        case GameState::PAUSED:
            printf("Paused!\n");
            if (is_key_pressed(Key::ESCAPE) or is_key_pressed(Key::SPACE)) {
                printf("Unpausing game\n");
                game.state = GameState::PLAYING;
            }
            pause_frame();
            break;
        case GameState::GAME_OVER:
            game_over_frame();
            if (is_key_pressed(Key::ESCAPE)) {
                quit();
            }
            else if(is_key_pressed(Key::SPACE)) {
                start();
            }
            break;
        default:
            printf("Unknown game state\n");
            break;
    }
}
