//
// Created by Luiz on 05/09/2024.
//

#ifndef SCORE_H
#define SCORE_H
#include <SDL.h>

class Score
{
public:
    //constructor
    Score(): sourcetext{0, 0, 55, 55},
    destinationtext{0, 0, 55, 55},
    Surface(nullptr),
    Texture(nullptr){}

    ~Score()
    {
        if (Texture)
        {
            SDL_DestroyTexture(Texture);
            Texture = nullptr;
        }
        if (Surface)
        {
            SDL_FreeSurface(Surface);
            Surface = nullptr;
        }
    }


    bool LoadScore(SDL_Window* window, SDL_Renderer* renderer, bool p1)
    {
        Surface = SDL_LoadBMP("../assets/Score.bmp");
        if (Surface == nullptr)
        {
            return false;
        }
        Texture = SDL_CreateTextureFromSurface(renderer, Surface);

        if (Texture == nullptr)
        {
            return false;
        }

        SDL_QueryTexture(Texture, nullptr, nullptr, &sourcetext.w, &sourcetext.h);
        SDL_GetWindowSize(window, &destinationtext.x, &destinationtext.y);

        destinationtext.x /= 2;
        if(p1)
        {
            destinationtext.y /= 10;
        }
        else
        {
            destinationtext.y -= 100;
        }
        return true;
    }

    bool RenderScore(SDL_Renderer* renderer, int actualscore)
    {
        if (Texture == nullptr)
        {
            return false; // Garantir que não tentamos renderizar uma textura nula
        }

        sourcetext.w = 55;
        sourcetext.x = 55 * actualscore;


        if (SDL_RenderCopy(renderer, Texture, &sourcetext, &destinationtext) != 0)
        {
            return false; // Retorna falso se houver falha em SDL_RenderCopy
        }

        return true;
    }


private:
    SDL_Surface* Surface;
    SDL_Texture* Texture;
    SDL_Rect destinationtext;
    SDL_Rect sourcetext;
};
#endif //SCORE_H
