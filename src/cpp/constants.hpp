#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants {
    // Rendering
    inline const int SCREEN_WIDTH = 640;
    inline const int SCREEN_HEIGHT = 480;

    // Asteroids
    inline const int ASTEROIDS_COOLDOWN = 10;
    inline const int MAX_ASTEROIDS = 10;
    inline const int ASTEROID_SPEED = 5;
    inline const int ASTEROID_ROTATION_SPEED = 5;
    inline const int ASTEROID_SIZE = 2;

    // Player
    inline const int SHOOTING_COOLDOWN = 10;
    inline const int BULLET_SPEED = 3;

    // calculate the center of the screen
    inline const int ZERO_X = SCREEN_WIDTH / 2;
    inline const int ZERO_Y = SCREEN_HEIGHT / 2;
}

namespace Key {
    inline const char UP = 119;
    inline const char DOWN = 115;
    inline const char LEFT = 97;
    inline const char RIGHT = 100;
    inline const char SPACE = 32;
    inline const char ESCAPE = 27;
    inline const char Q = 113;
    inline const char R = 114;
}

#endif // CONSTANTS_HPP