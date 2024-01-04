/**
 * @brief Reset the game state
 * @details This function is called when the game is reset or initialized.
*/
void reset();


/**
 * @brief Starts a new game
 * @details This function is called when the game is started.
 *          It first resets the game state before starting the game.
*/
void start();


/**
 * @brief Pause the game
 * @details This function is called when the game is paused.
*/
void pause();


/**
 * @brief Resume the game
 * @details This function is called when the game is resumed.
*/
void resume();


/**
 * @brief Quit the game
 * @details This function is called when a player quits mid-game.
*/
void quit();


extern "C" {

    /**
     * @brief Initialize the game state
     * @details This function is called once when the game starts.
    */
    EMSCRIPTEN_KEEPALIVE
    void init();

    /**
     * @brief Update the game state
     * @details This function is called once per frame.
    */
    EMSCRIPTEN_KEEPALIVE
    void get_frame();

}