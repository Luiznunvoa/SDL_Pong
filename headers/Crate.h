#ifndef CRATE_H
#define CRATE_H

#include <SDL.h>

class Crate
{
public:
    Crate() : pTempSurface(nullptr), m_pTexture(nullptr), sourceRectangle{0, 0, 0, 0}, destinationRectangle{0, 0, 0, 0}, incrementx(5), incrementy(5) {}

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
        if((incrementx + incrementy) != 0)
        {
            incrementx = NULL;
            incrementy = NULL;
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
        destinationRectangle.x = 0;
        destinationRectangle.y = 0;
        return true;
    }

    void UpdateRectanglePosition(SDL_Window* window)
    {
        int width, height;

        // Obter o tamanho da janela
        SDL_GetWindowSize(window, &width, &height);

        // Verificar colisão com as bordas horizontais
        if (((destinationRectangle.x + destinationRectangle.w) == width && incrementx > 0) ||
            (destinationRectangle.x == 0 && incrementx < 0))
        {
            // Reverter a direção no eixo X
            incrementx = -incrementx;
        }

        // Verificar colisão com as bordas verticais
        if (((destinationRectangle.y + destinationRectangle.h) == height && incrementy > 0) ||
            (destinationRectangle.y == 0 && incrementy < 0))
        {
            // Reverter a direção no eixo Y
            incrementy = -incrementy;
        }

        // Atualizar a posição do retângulo
        destinationRectangle.x += incrementx;
        destinationRectangle.y += incrementy;

        // Garantir que o retângulo não ultrapasse os limites da janela
        if (destinationRectangle.x < 0)
            destinationRectangle.x = 0;
        else if (destinationRectangle.x + destinationRectangle.w > width)
            destinationRectangle.x = width - destinationRectangle.w;

        if (destinationRectangle.y < 0)
            destinationRectangle.y = 0;
        else if (destinationRectangle.y + destinationRectangle.h > height)
            destinationRectangle.y = height - destinationRectangle.h;
    }

    bool RenderCrate(SDL_Renderer* renderer)
    {
        if (m_pTexture == nullptr)
        {
            return false; // Garantir que não tentamos renderizar uma textura nula
        }

        // Define a posição e o tamanho do destinationRectangle
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
    int incrementx;
    int incrementy;
};

#endif // CRATE_H
