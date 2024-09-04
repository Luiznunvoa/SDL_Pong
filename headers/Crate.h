#ifndef CRATE_H
#define CRATE_H

#include <SDL.h>

class Crate
{
public:
    Crate() : pTempSurface(nullptr), m_pTexture(nullptr), sourceRectangle{0, 0, 0, 0}, destinationRectangle{0, 0, 0, 0} {}

    ~Crate()
    {
        if (m_pTexture)
        {
            SDL_DestroyTexture(m_pTexture);
            m_pTexture = nullptr;
        }
        if (pTempSurface)
        {
            SDL_FreeSurface(pTempSurface);
            pTempSurface = nullptr;
        }
    }

    bool LoadCrate(SDL_Renderer* renderer) {
        pTempSurface = SDL_LoadBMP("../assets/rider.bmp");
        if (pTempSurface == nullptr)
        {
            return false;
        }

        m_pTexture = SDL_CreateTextureFromSurface(renderer, pTempSurface);

        if (m_pTexture == nullptr)
        {
            return false;
        }

        // Defina o tamanho do sourceRectangle após carregar a textura
        SDL_QueryTexture(m_pTexture, nullptr, nullptr, &sourceRectangle.w, &sourceRectangle.h);
        return true;
    }

    bool RenderCrate(SDL_Renderer* renderer)
    {
        if (m_pTexture == nullptr)
        {
            return false; // Garantir que não tentamos renderizar uma textura nula
        }

        // Define a posição e o tamanho do destinationRectangle
        destinationRectangle.x = 150;
        destinationRectangle.y = 150;
        destinationRectangle.w = sourceRectangle.w;
        destinationRectangle.h = sourceRectangle.h;

        if (SDL_RenderCopy(renderer, m_pTexture, &sourceRectangle, &destinationRectangle) != 0)
        {
            return false; // Retorna falso se houver falha em SDL_RenderCopy
        }

        return true;
    }

private:
    SDL_Surface* pTempSurface;
    SDL_Texture* m_pTexture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
};

#endif // CRATE_H
