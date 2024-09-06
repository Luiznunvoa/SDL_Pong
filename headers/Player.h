#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

/**
 * @brief Class representing a Player in the game.
 *
 * This class encapsulates the logic for loading, updating, and rendering the player.
 */
class Player
{
public:
    /**
     * @brief Constructor for the Player class.
     *
     * Initializes the player data with default values.
     */
    Player() : data{
        nullptr,
        nullptr,
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        0} {}

    /**
     * @brief Destructor for the Player class.
     *
     * Cleans up SDL resources.
     */
    ~Player()
    {
        if (data.Texture) SDL_DestroyTexture(data.Texture);
        if (data.Surface) SDL_FreeSurface(data.Surface);
    }

    /**
     * @brief Loads the player texture.
     *
     * @param window The SDL window.
     * @param renderer The SDL renderer.
     * @param p1 Boolean indicating if the player is player 1.
     * @return True if the player texture is loaded successfully, false otherwise.
     */
    bool LoadPlayer(SDL_Window* window, SDL_Renderer* renderer, bool p1)
    {
        data.Surface = SDL_LoadBMP("../assets/Player.bmp");
        if (!data.Surface) return false;

        data.Texture = SDL_CreateTextureFromSurface(renderer, data.Surface);
        if (!data.Texture) return false;

        SDL_QueryTexture(data.Texture, nullptr, nullptr, &data.sourceplayer.w, &data.sourceplayer.h);
        SDL_GetWindowSize(window, &data.destinationplayer.x, &data.destinationplayer.y);
        data.destinationplayer.x = p1 ? 50 : data.destinationplayer.x - 100;
        data.destinationplayer.y /= 4;

        return true;
    }

    /**
     * @brief Updates the player position.
     *
     * @param window The SDL window.
     * @param Up Boolean indicating if the player is moving up.
     * @param Down Boolean indicating if the player is moving down.
     * @return The updated destination rectangle of the player.
     */
    SDL_Rect UpdatePlayerPosition(SDL_Window* window, bool Up, bool Down)
    {
        data.currentdirection = Up ? -5 : (Down ? 5 : 0);

        int windowHeight;
        SDL_GetWindowSize(window, nullptr, &windowHeight);

        if ((data.currentdirection < 0 && data.destinationplayer.y > 0) ||
            (data.currentdirection > 0 && (data.destinationplayer.y + data.destinationplayer.h) < windowHeight))
        {
            data.destinationplayer.y += data.currentdirection;
        }
        return data.destinationplayer;
    }

    /**
     * @brief Renders the player.
     *
     * @param renderer The SDL renderer.
     * @return True if the player is rendered successfully, false otherwise.
     */
    bool Renderplayer(SDL_Renderer* renderer)
    {
        if (!data.Texture) return false;

        data.destinationplayer.w = data.sourceplayer.w;
        data.destinationplayer.h = data.sourceplayer.h;

        return SDL_RenderCopy(renderer, data.Texture, &data.sourceplayer, &data.destinationplayer) == 0;
    }

private:
    /**
     * @brief Struct to hold player data.
     */
    struct PlayerData
    {
        SDL_Surface* Surface; ///< The SDL surface for the player texture.
        SDL_Texture* Texture; ///< The SDL texture for the player.
        SDL_Rect sourceplayer; ///< The source rectangle for the player texture.
        SDL_Rect destinationplayer; ///< The destination rectangle for the player texture.
        int currentdirection; ///< The current direction of the player movement.
    } data;
};

#endif // PLAYER_H