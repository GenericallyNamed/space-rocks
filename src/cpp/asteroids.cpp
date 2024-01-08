#include <cmath>
#include <stdio.h>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include "asteroids.hpp"
#include "constants.hpp"
#include "input.hpp"
#include "render.hpp"
#include "state.hpp"


using emscripten::val;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_ASTEROIDS = 10;


void send_signal(const char* signal) {
    val window = val::global("window");
    window.call<val>("sendSignal", val(signal));
}


void send_signal(const char* signal, int value) {
    val window = val::global("window");
    window.call<val>("sendSignal", val(signal), val(value));
}


void reset() {
    game.player.x = Constants::ZERO_X;
    game.player.y = Constants::ZERO_Y;
    game.player.rotation = 0;
    game.player.dx = 0;
    game.player.dy = 0;
    game.player.lives = 3;
    game.player.lives_updated = true;
    game.asteroids.clear();
    game.bullets.clear();
    game.score = 0;
    game.score_updated = true;
    reset_render();
}


void start() {
    reset();
    game.state = GameState::PLAYING;
}

void quit() {
    reset();
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
        generate_rocket_exhaust(game.player.x, game.player.y, game.player.rotation, game.player.dx, game.player.dy);
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
        game.add_bullet();
    }
    if(game.player.x < -10) {
        game.player.x = Constants::SCREEN_WIDTH + 10;
    }
    else if(game.player.x > Constants::SCREEN_WIDTH + 10) {
        game.player.x = -10;
    }
    if(game.player.y < -10) {
        game.player.y = Constants::SCREEN_HEIGHT + 10;
    }
    else if(game.player.y > Constants::SCREEN_HEIGHT + 10) {
        game.player.y = -10;
    }

    game.player.x += game.player.dx;
    game.player.y += game.player.dy;
    simulate_bullets();
    game.check_bullets();
    // ======= Asteroid simulation =======
    simulate_asteroids();
    game.check_asteroids();


    // Rendering elements
    draw_stars();
    draw_rocket_exhaust();
    draw_player(game.player.x, game.player.y, game.player.rotation);
    for(auto asteroid : game.asteroids) {
        draw_asteroid(asteroid.x, asteroid.y, asteroid.rotation);
    }
    for(auto bullet : game.bullets) {
        draw_bullet(bullet.x, bullet.y);
    }
    if(game.player.lives == 0) {
        game.state = GameState::GAME_OVER;
        send_signal("gameOver");
    }

    if(game.player.lives_updated) {
        send_signal("livesUpdated", game.player.lives);
        game.player.lives_updated = false;
    }
    if(game.score_updated) {
        send_signal("scoreUpdated", game.score);
        game.score_updated = false;
    }

}


void menu_frame() {
    draw_stars();
    draw_player(game.player.x, game.player.y, game.player.rotation);
}


void game_over_frame() {
    draw_stars();
}


void pause_frame() {
    draw_stars();
    draw_player(game.player.x, game.player.y, game.player.rotation);
    // draw asteroids
    for(auto asteroid : game.asteroids) {
        draw_asteroid(asteroid.x, asteroid.y, asteroid.rotation);
    }
    // draw bullets
    for(auto bullet : game.bullets) {
        draw_bullet(bullet.x, bullet.y);
    }
}


void init() {
    printf("Attempting to initialize game\n");
    reset();
    init_stars();
    printf("Game is initialized!\n");
}

void get_frame() {
    clear_canvas();
    switch (game.state) {
        case GameState::MENU:
            if (is_key_pressed(Key::SPACE)) {
                start();
                send_signal("start");
            }
            menu_frame();
            break;
        case GameState::PLAYING:
            if (game.player.lives == 0) {
                game.state = GameState::GAME_OVER;
            }
            if (is_key_pressed(Key::ESCAPE)) {
                game.state = GameState::PAUSED;
                send_signal("pause");
            }
            play_frame();
            // render_test();
            break;
        case GameState::PAUSED:
            if (is_key_pressed(Key::ESCAPE) or is_key_pressed(Key::SPACE)) {
                game.state = GameState::PLAYING;
                send_signal("resume");
            }
            if (is_key_pressed(Key::Q)) {
                game.state = GameState::MENU;
                send_signal("quit");
                quit();
            }
            if (is_key_pressed(Key::R)) {
                start();
                send_signal("restart");
            }
            pause_frame();
            break;
        case GameState::GAME_OVER:
            game_over_frame();
            if (is_key_pressed(Key::ESCAPE)) {
                game.state = GameState::MENU;
                send_signal("quit");
                quit();
            }
            if (is_key_pressed(Key::Q)) {
                game.state = GameState::MENU;
                send_signal("quit");
                quit();
            }
            else if(is_key_pressed(Key::SPACE)) {
                start();
                send_signal("restart");
            }
            break;
        default:
            printf("Unknown game state\n");
            break;
    }
}
