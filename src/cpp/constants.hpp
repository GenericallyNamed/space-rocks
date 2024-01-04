#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants {
    // Rendering
    inline const int SCREEN_WIDTH = 640;
    inline const int SCREEN_HEIGHT = 480;

    // Gameplay
    inline const int ASTEROIDS_COOLDOWN = 300;
    inline const int MAX_ASTEROIDS = 10;
    inline const int SHOOTING_COOLDOWN = 10;
    inline const int BULLET_SPEED = 3;
    inline const int ASTEROID_SPEED = 5;
    inline const int ASTEROID_ROTATION_SPEED = 5;

    // calculate the center of the screen
    inline const int ZERO_X = SCREEN_WIDTH / 2;
    inline const int ZERO_Y = SCREEN_HEIGHT / 2;
}

#endif // CONSTANTS_HPP