#ifndef SCORE_H
#define SCORE_H
#include <SDL.h>
#include <string>

/**
 * @brief Class representing the Score in the game.
 *
 * This class encapsulates the logic for loading and rendering the score.
 */
class Score
{
public:
    /**
     * @brief Constructor for the Score class.
     *
     * Initializes the score data with default values.
     */
    Score() : sourcetext{0, 0, 55, 55}, destinationtext{0, 0, 55, 55}, Surface(nullptr), Texture(nullptr) {}

    /**
     * @brief Destructor for the Score class.
     *
     * Cleans up SDL resources.
     */
    ~Score()
    {
        if (Texture) SDL_DestroyTexture(Texture);
        if (Surface) SDL_FreeSurface(Surface);
    }

    /**
     * @brief Loads the score texture.
     *
     * @param window The SDL window.
     * @param renderer The SDL renderer.
     * @param p1 Boolean indicating if the score is for player 1.
     * @return True if the score texture is loaded successfully, false otherwise.
     */
    bool LoadScore(SDL_Window* window, SDL_Renderer* renderer, bool p1)
    {
        Surface = SDL_LoadBMP("../assets/Score.bmp");
        if (!Surface) return false;

        Texture = SDL_CreateTextureFromSurface(renderer, Surface);
        if (!Texture) return false;

        SDL_QueryTexture(Texture, nullptr, nullptr, &sourcetext.w, &sourcetext.h);
        SDL_GetWindowSize(window, &destinationtext.x, &destinationtext.y);

        destinationtext.x /= 2;
        destinationtext.y = p1 ? destinationtext.y / 10 : destinationtext.y - 100;

        return true;
    }

    /**
     * @brief Renders the score.
     *
     * @param renderer The SDL renderer.
     * @param actualscore The current score to be rendered.
     * @return True if the score is rendered successfully, false otherwise.
     */
    bool RenderScore(SDL_Renderer* renderer, int actualscore)
    {
        if (!Texture) return false;

        std::string scoreStr = std::to_string(actualscore);
        int digitWidth = 55;

        for (size_t i = 0; i < scoreStr.size(); ++i)
        {
            int digit = scoreStr[i] - '0';
            sourcetext.x = digit * digitWidth;
            sourcetext.w = digitWidth;

            destinationtext.x = (destinationtext.x / 2) + (i * digitWidth) + 320;
            destinationtext.w = digitWidth;

            if (SDL_RenderCopy(renderer, Texture, &sourcetext, &destinationtext) != 0) return false;
        }

        return true;
    }

private:
    SDL_Surface* Surface; ///< The SDL surface for the score texture.
    SDL_Texture* Texture; ///< The SDL texture for the score.
    SDL_Rect destinationtext; ///< The destination rectangle for the score texture.
    SDL_Rect sourcetext; ///< The source rectangle for the score texture.
};

#endif // SCORE_H