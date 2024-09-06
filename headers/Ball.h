#ifndef BALL_H
#define BALL_H

#include <SDL.h>

/**
 * @brief Class representing a Ball in the game.
 *
 * This class encapsulates the logic for loading, updating, and rendering the ball.
 */
class Ball
{
public:
    /**
     * @brief Constructor for the Ball class.
     *
     * Initializes the ball data with default values.
     */
    Ball() : data{
        nullptr,
        nullptr,
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        5, 5} {}

    /**
     * @brief Destructor for the Ball class.
     *
     * Cleans up SDL resources.
     */
    ~Ball()
    {
        if (data.Texture) SDL_DestroyTexture(data.Texture);
        if (data.Surface) SDL_FreeSurface(data.Surface);
    }

    /**
     * @brief Loads the ball texture.
     *
     * @param window The SDL window.
     * @param renderer The SDL renderer.
     * @return True if the ball texture is loaded successfully, false otherwise.
     */
    bool LoadBall(SDL_Window* window, SDL_Renderer* renderer)
    {
        data.Surface = SDL_LoadBMP("../assets/rider.bmp");
        if (!data.Surface) return false;

        data.Texture = SDL_CreateTextureFromSurface(renderer, data.Surface);
        if (!data.Texture) return false;

        SDL_QueryTexture(data.Texture, nullptr, nullptr, &data.sourceball.w, &data.sourceball.h);
        SDL_GetWindowSize(window, &data.destinationball.x, &data.destinationball.y);
        data.destinationball.x /= 2;
        data.destinationball.y /= 2;

        return true;
    }

    /**
     * @brief Updates the ball position.
     *
     * @param window The SDL window.
     * @param player1Rect The rectangle representing player 1.
     * @param player2Rect The rectangle representing player 2.
     * @param score1 Pointer to the score of player 1.
     * @param score2 Pointer to the score of player 2.
     */
    void UpdateballPosition(SDL_Window* window, SDL_Rect player1Rect, SDL_Rect player2Rect, int* score1, int* score2)
    {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        data.destinationball.w = data.sourceball.w;
        data.destinationball.h = data.sourceball.h;

        if (data.destinationball.x + data.destinationball.w >= width || data.destinationball.x <= 0)
        {
            data.incrementx = -data.incrementx;
            data.destinationball.x = width / 2;
            data.destinationball.y = height / 2;
            if (data.destinationball.x <= 0) (*score2)++;
            else (*score1)++;
        }

        if (data.destinationball.y + data.destinationball.h >= height || data.destinationball.y <= 0)
        {
            data.incrementy = -data.incrementy;
        }

        if (CheckCollisionWithPlayer(player1Rect) || CheckCollisionWithPlayer(player2Rect))
        {
            data.incrementx = -data.incrementx;
        }

        data.destinationball.x += data.incrementx;
        data.destinationball.y += data.incrementy;
    }

    /**
     * @brief Renders the ball.
     *
     * @param renderer The SDL renderer.
     * @return True if the ball is rendered successfully, false otherwise.
     */
    bool RenderBall(SDL_Renderer* renderer)
    {
        if (!data.Texture) return false;

        data.destinationball.w = data.sourceball.w;
        data.destinationball.h = data.sourceball.h;

        return SDL_RenderCopy(renderer, data.Texture, &data.sourceball, &data.destinationball) == 0;
    }

private:
    /**
     * @brief Struct to hold ball data.
     */
    struct BallData
    {
        SDL_Surface* Surface; ///< The SDL surface for the ball texture.
        SDL_Texture* Texture; ///< The SDL texture for the ball.
        SDL_Rect sourceball; ///< The source rectangle for the ball texture.
        SDL_Rect destinationball; ///< The destination rectangle for the ball texture.
        int incrementx; ///< The increment value for the ball's x position.
        int incrementy; ///< The increment value for the ball's y position.
    } data;

    /**
     * @brief Checks collision with a player.
     *
     * @param playerRect The rectangle representing the player.
     * @return True if there is a collision, false otherwise.
     */
    bool CheckCollisionWithPlayer(SDL_Rect playerRect)
    {
        return data.destinationball.x < playerRect.x + playerRect.w &&
               data.destinationball.x + data.destinationball.w > playerRect.x &&
               data.destinationball.y < playerRect.y + playerRect.h &&
               data.destinationball.y + data.destinationball.h > playerRect.y;
    }
};

#endif // BALL_H