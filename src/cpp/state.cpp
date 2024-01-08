#include <stdio.h>
#include <emscripten/emscripten.h>
#include "constants.hpp"
#include "state.hpp"

Game game = Game();

void Game::add_asteroid() {
    float x, y;
    float dx, dy;
    float rotation;

    /**
     * @brief The randomly chosen side from which to spawn the asteroid
    */
    int side = std::rand() % 4;

    // Now we'll generate the starting coordinates, placing it randomly
    // on one of the four sides of the screen.
    switch(side) {
        case 0: // Top
            x = std::rand() % Constants::SCREEN_WIDTH;
            y = 0;
            break;
        case 1: // Right
            x = Constants::SCREEN_WIDTH;
            y = std::rand() % Constants::SCREEN_HEIGHT;
            break;
        case 2: // Bottom
            x = std::rand() % Constants::SCREEN_WIDTH;
            y = Constants::SCREEN_HEIGHT;
            break;
        case 3: // Left
            x = 0;
            y = std::rand() % Constants::SCREEN_HEIGHT;
            break;
    }

    // Now we'll generate the starting velocity, pointing towards the center
    // of the screen.
    // Use a bit of trig to calculate a vector pointing from x, y to the center
    // Note that the center is defined with the constants
    dx = Constants::ZERO_X - x;
    dy = Constants::ZERO_Y - y;
    // Scale this vector so it has a length of 1
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;


    asteroids.push_back(Asteroid(x, y, dx, dy, 0));
    num_asteroids++;
}


void Game::check_asteroids() {
    for(auto it = asteroids.begin(); it != asteroids.end();) {
        float x = it->x;
        float y = it->y;
        // ==== Check if out of bounds ============
        if(x < 0 or x > Constants::SCREEN_WIDTH or y < 0 or y > Constants::SCREEN_HEIGHT) {
            it = asteroids.erase(it);
            continue;
        }

        // ==== Check if player collision =========
        // Note we do this every frame because player can collide arbitrarily
        if(std::sqrt(std::pow(player.x - x, 2.0) + std::pow(player.y - y, 2.0)) < 10) {
            it = asteroids.erase(it);
            player.lives--;
            player.lives_updated = true;
            continue;
        }


        // ==== Check if bullet collision =========
        // Can I optimize this better? Perhaps!
        bool destroyed = false;
        for(auto b_it = bullets.begin(); b_it != bullets.end();) {
            float bullet_x = b_it->x;
            float bullet_y = b_it->y;
            if(std::sqrt(std::pow(bullet_x - x, 2.0) + std::pow(bullet_y - y, 2.0)) < 10) {
                destroyed = true;
                b_it = bullets.erase(b_it);
                break;
            }   
            b_it++;
        }

        if(destroyed) {
            it = asteroids.erase(it);
            score += 1;
            score_updated = true;
            continue;
        }
        
        it++;
    }
}


void Game::add_bullet() {
    float x, y;
    float dx, dy;

    // The bullet will start just ahead of the player's position, incorporating
    // the player's rotation to calculate it's velocity.
    x = player.x + 11 * std::sin(player.rotation);
    y = player.y - 11 * std::cos(player.rotation);

    // Calcualte dx and dy
    dx = Constants::BULLET_SPEED * std::sin(player.rotation);
    dy = - Constants::BULLET_SPEED * std::cos(player.rotation);

    bullets.push_back(Bullet(x, y, dx, dy));
}


void Game::check_bullets() {
    for(auto it = bullets.begin(); it != bullets.end();) {
        float x = it->x;
        float y = it->y;
        // Check if bullet is out of bounds
        if(x < 0 or x > Constants::SCREEN_WIDTH or y < 0 or y > Constants::SCREEN_HEIGHT) {
            it = bullets.erase(it);
            continue;
        }
        it++;
    }
}


extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void init_game() {
        game = Game();
    }

    EMSCRIPTEN_KEEPALIVE
    void reset_game() {
        game = Game();
    }

    EMSCRIPTEN_KEEPALIVE
    void delete_game() {
        // delete game;
    }

}