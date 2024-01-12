#include <list>
#include <cmath>
#include <emscripten/emscripten.h>
// I need to be able to be able to store and access the game state.

// The state of the game defines the onscreen behavior.
enum class GameState
{
    // Simulating asteroids in the background while the player is in the menu
    MENU,
    // The player is playing the game
    PLAYING,
    // The player has paused the game
    PAUSED,
    // The player has lost all their lives
    GAME_OVER
};

/**
 * @brief The state of the game
 * @details All information about the game state is stored here.
 */
class Game
{
public:
    Game() = default;
    ~Game() = default;

    /**
     * @brief Add an asteroid to the game.
     */
    void add_asteroid();

    /**
     * @brief Check liveliness of asteroids.
     */
    void check_asteroids();

    /**
     * @brief Add a bullet to the game.
     */
    void add_bullet();

    /**
     * @brief Check liveliness of bullets.
     */
    void check_bullets();

    /**
     * @brief Represents the game's score.
     */
    int score = 0;
    bool score_updated = false;
    GameState state = GameState::MENU;

    /**
     * @brief Represents the player's ship.
     */
    struct Player
    {
        Player() = default;
        ~Player() = default;
        // Position and movement
        float x = 0;
        float y = 0;
        float rotation = 0;
        float dx = 0;
        float dy = 0;

        // Player status
        int lives = 3;
        bool lives_updated = false;
        int shooting_cooldown = 0;
    };

    Player player = Player();

    /**
     * @brief Represents the bullets in the game.
     */
    struct Bullet
    {
        Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}

        float x = 0;
        float y = 0;
        float dx = 0;
        float dy = 0;
    };

    std::list<Bullet> bullets;

    /**
     * @brief Represents the asteroids in the game.
     */
    struct Asteroid
    {
        Asteroid(float x, float y, float dx, float dy, float rotation) : x(x), y(y), dx(dx), dy(dy), rotation(rotation) {
            for(int i = 0; i < 8; i++) {
                lengths[i] = std::rand() % 4 + 2;
            }
        }
        ~Asteroid() = default;

        float x = 0;
        float y = 0;
        float dx = 0;
        float dy = 0;
        float rotation = 0;

        // Lengths of 8 vectors, like 8 points on a compass. Randomly generated.
        // Range of 2 to 5.
        float lengths[8];
    };

    std::list<Asteroid> asteroids;
    int asteroids_cooldown = 0;
    int num_asteroids = 0;
};

extern Game game;
