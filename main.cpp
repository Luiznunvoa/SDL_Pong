#include <SDL.h>
#include "headers/Game.h"

// Define o intervalo de ticks (tempo entre frames) para 30ms (~33 FPS)
#define TICK_INTERVAL 30

/**
 * @brief Calculates the time remaining until the next frame.
 *
 * This function calculates the time left until the next frame based on the current
 * time and the specified next frame time.
 *
 * @param next_time The time of the next frame in milliseconds.
 * @return The time remaining until the next frame in milliseconds.
 */
static Uint32 time_left(Uint32 next_time)
{
    Uint32 now = SDL_GetTicks();  // Pega o tempo atual
    return (next_time <= now) ? 0 : next_time - now;
}

/**
 * @brief The main function of the game.
 *
 * This function initializes the game manager, sets up the SDL window and renderer,
 * and runs the main game loop. It handles events, updates the game state, and renders
 * the game elements. The game loop runs until the game is no longer running.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 if the game exits successfully, -1 if there is an initialization error.
 */

int main(int argc, char* argv[])
{
    // Inicializes the game manager
    Game* gameManager = new Game();

    // Inicializes the SDL library
    if (!gameManager->init(SDL_INIT_VIDEO, "MyWindow",
        100, 50, 1280, 960,
        SDL_WINDOW_SHOWN
        ))
    {
        // If there is an error initializing the game, return -1
        return -1;
    }

    gameManager->setRunning(true);  // Sets the game to running state

    Uint32 next_time = SDL_GetTicks() + TICK_INTERVAL;  // Tempo do próximo frame

    // Main Game loop
    while (gameManager->isRunning())
    {
        gameManager->handleEvents();  // Process events
        gameManager->update();        // Calculates the game state
        gameManager->render();        // Render the game elements

        // Wait until the next frame
        SDL_Delay(time_left(next_time));
        next_time += TICK_INTERVAL;  // Calculating the time of the next frame
    }

    // Free the memory from the gamemanager
    delete gameManager;

    // Quit the SDL library
    SDL_Quit();

    return 0;
}