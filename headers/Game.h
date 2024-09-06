#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <memory>
#include "Ball.h"
#include "Player.h"
#include "Score.h"

/**
 * @brief Struct for window state.
 *
 * This struct holds the SDL window and renderer.
 */
struct WindowState
{
    SDL_Window* window = nullptr; ///< Pointer to the SDL window.
    SDL_Renderer* renderer = nullptr; ///< Pointer to the SDL renderer.
};

/**
 * @brief Struct for game state.
 *
 * This struct holds the running state and the actual score of the game.
 */
struct GameState
{
    bool Running = false; ///< Boolean indicating if the game is running.
    int actualscore1 = 0; ///< The current score of the player 1.
    int actualscore2 = 0; ///< The current score of the player 2.
};

/**
 * @brief Struct for game elements.
 *
 * This struct holds unique pointers to the game elements: ball, score, and players.
 */
struct GameElements
{
    std::unique_ptr<Ball> ball; ///< Unique pointer to the Ball object.
    std::unique_ptr<Score> score1; ///< Unique pointer to the Score object.
    std::unique_ptr<Score> score2; ///< Unique pointer to the Score object.
    std::unique_ptr<Player> player1; ///< Unique pointer to the first Player object.
    std::unique_ptr<Player> player2; ///< Unique pointer to the second Player object.
};

/**
 * @brief Struct for input state.
 *
 * This struct holds the state of the input flags for player 1 and player 2.
 */
struct InputState
{
    bool player1UpPressed = false; ///< Boolean indicating if player 1's up key is pressed.
    bool player1DownPressed = false; ///< Boolean indicating if player 1's down key is pressed.
    bool player2UpPressed = false; ///< Boolean indicating if player 2's up key is pressed.
    bool player2DownPressed = false; ///< Boolean indicating if player 2's down key is pressed.

    /**
     * @brief Handles key input events and updates the input state.
     *
     * This function processes SDL key events and updates the state of the
     * input flags for player 1 and player 2 based on the key pressed or released.
     *
     * @param event The SDL_Event containing the key event information.
     * @param state The state of the key (true if pressed, false if released).
     */
    void handleKeyInput(const SDL_Event& event, bool state)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_w:
                player1UpPressed = state;
                break;
            case SDLK_s:
                player1DownPressed = state;
                break;
            case SDLK_UP:
                player2UpPressed = state;
                break;
            case SDLK_DOWN:
                player2DownPressed = state;
                break;
            default:
                break;
        }
    }
};

/**
 * @brief Class representing the game.
 *
 * This class encapsulates the game logic, including initialization, event handling,
 * updating game state, and rendering.
 */
class Game
{
public:
    /**
     * @brief Constructor for the Game class.
     *
     * Initializes the game elements and input state.
     */
    Game():
    windowState{},
    gamestate{},
    gameElements{
        std::make_unique<Ball>(),
        std::make_unique<Score>(),
        std::make_unique<Score>(),
        std::make_unique<Player>(),
        std::make_unique<Player>()},
    inputState{}
    {}

    /**
     * @brief Destructor for the Game class.
     *
     * Cleans up SDL resources.
     */
    ~Game()
    {
        if (windowState.renderer)
        {
            SDL_DestroyRenderer(windowState.renderer);
        }
        if (windowState.window)
        {
            SDL_DestroyWindow(windowState.window);
        }
        SDL_Quit();
    }

    /**
     * @brief Sets the running state of the game.
     *
     * @param running Boolean indicating if the game should be running.
     */
    void setRunning(bool running)
    {
        gamestate.Running = running;
    }

    /**
     * @brief Checks if the game is running.
     *
     * @return True if the game is running, false otherwise.
     */
    [[nodiscard]] bool isRunning() const
    {
        return gamestate.Running;
    }

    /**
     * @brief Initializes SDL and sets up the window and renderer.
     *
     * @param initflag The SDL initialization flags.
     * @param title The title of the window.
     * @param xpos The x position of the window.
     * @param ypos The y position of the window.
     * @param width The width of the window.
     * @param height The height of the window.
     * @param windowflag The SDL window flags.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
    {
        if (SDL_Init(initflag) < 0)
        {
            return false;
        }

        windowState.window = SDL_CreateWindow(title, xpos, ypos, width, height, windowflag);
        if (!windowState.window)
        {
            SDL_Quit();
            return false;
        }

        windowState.renderer = SDL_CreateRenderer(windowState.window, -1, 0);
        if (!windowState.renderer)
        {
            SDL_DestroyWindow(windowState.window);
            SDL_Quit();
            return false;
        }

        if (!gameElements.ball->LoadBall(windowState.window, windowState.renderer) ||
            !gameElements.player1->LoadPlayer(windowState.window, windowState.renderer, true) ||
            !gameElements.player2->LoadPlayer(windowState.window, windowState.renderer, false) ||
            !gameElements.score1->LoadScore(windowState.window, windowState.renderer, true) ||
            !gameElements.score2->LoadScore(windowState.window, windowState.renderer, false)
        )
        {
            return false;
        }

        return true;
    }

    /**
     * @brief Handles SDL events.
     *
     * Processes SDL events such as quitting and key presses.
     */
    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gamestate.Running = false;
            }
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                inputState.handleKeyInput(event, event.type == SDL_KEYDOWN);
            }
        }
    }

    /**
     * @brief Updates the game state.
     *
     * Updates the positions of the players and the ball, and updates the score.
     */
    void update()
    {
        SDL_Rect rect1 = gameElements.player1->UpdatePlayerPosition(windowState.window, inputState.player1UpPressed, inputState.player1DownPressed);
        SDL_Rect rect2 = gameElements.player2->UpdatePlayerPosition(windowState.window, inputState.player2UpPressed, inputState.player2DownPressed);
        gameElements.ball->UpdateballPosition(windowState.window, rect1, rect2, &gamestate.actualscore1, &gamestate.actualscore2);
    }

    /**
     * @brief Renders the game.
     *
     * Clears the renderer, renders the game elements, and presents the renderer.
     */
    void render()
    {
        SDL_SetRenderDrawColor(windowState.renderer, 0, 0, 0, 255);
        SDL_RenderClear(windowState.renderer);

        if (
            !gameElements.score1->RenderScore(windowState.renderer, gamestate.actualscore1) ||
            !gameElements.score2->RenderScore(windowState.renderer, gamestate.actualscore2) ||
            !gameElements.ball->RenderBall(windowState.renderer) ||
            !gameElements.player1->Renderplayer(windowState.renderer) ||
            !gameElements.player2->Renderplayer(windowState.renderer)
            )
        {
            gamestate.Running = false;
        }

        SDL_RenderPresent(windowState.renderer);
    }

private:
    WindowState windowState; ///< The state of the window.
    GameState gamestate; ///< The state of the game.
    GameElements gameElements; ///< The elements of the game.
    InputState inputState; ///< The state of the player input.
};

#endif // GAME_H