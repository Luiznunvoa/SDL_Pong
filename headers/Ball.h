#ifndef BALL_H
#define BALL_H

#include <SDL.h>

// Classe do retangulo
class Ball
{
public:
    Ball() : Surface(nullptr), Texture(nullptr), sourceball{0, 0, 0, 0}, destinationball{0, 0, 0, 0}, incrementx(5), incrementy(5) {}

    ~Ball()
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

    bool LoadBall(SDL_Window* window, SDL_Renderer* renderer) {
        Surface = SDL_LoadBMP("../assets/rider.bmp");
        if (Surface == nullptr)
        {
            return false;
        }

        Texture = SDL_CreateTextureFromSurface(renderer, Surface);

        if (Texture == nullptr)
        {
            return false;
        }

        // Defina o tamanho do sourceball após carregar a textura
        SDL_QueryTexture(Texture, nullptr, nullptr, &sourceball.w, &sourceball.h);
        SDL_GetWindowSize(window, &destinationball.x, &destinationball.y);
        destinationball.x /= 2;
        destinationball.y /= 2;

        return true;
    }

    //Altera a posição do retangulo
    void UpdateballPosition(SDL_Window* window)
    {
        int width, height;

        // Obter o tamanho da janela
        SDL_GetWindowSize(window, &width, &height);

        // Verificar colisão com as bordas horizontais
        if (((destinationball.x + destinationball.w) == width && incrementx > 0) ||
            (destinationball.x == 0 && incrementx < 0))
        {
            // Reverter a direção no eixo X
            incrementx = -incrementx;

            destinationball.x = (width / 2);
            destinationball.y = (height / 2);
        }

        // Verificar colisão com as bordas verticais
        if (((destinationball.y + destinationball.h) == height && incrementy > 0) ||
            (destinationball.y == 0 && incrementy < 0))
        {
            // Reverter a direção no eixo Y
            incrementy = -incrementy;
        }

        // Atualizar a posição do retângulo
        destinationball.x += incrementx;
        destinationball.y += incrementy;

        // Garantir que o retângulo não ultrapasse os limites da janela
        if (destinationball.x < 0)
            destinationball.x = 0;
        else if (destinationball.x + destinationball.w > width)
            destinationball.x = width - destinationball.w;

        if (destinationball.y < 0)
            destinationball.y = 0;
        else if (destinationball.y + destinationball.h > height)
            destinationball.y = height - destinationball.h;
    }

    //Renderiza o retangulo
    bool RenderBall(SDL_Renderer* renderer)
    {
        if (Texture == nullptr)
        {
            return false; // Garantir que não tentamos renderizar uma textura nula
        }

        // Define a posição e o tamanho do destinationball
        destinationball.w = sourceball.w;
        destinationball.h = sourceball.h;

        if (SDL_RenderCopy(renderer, Texture, &sourceball, &destinationball) != 0)
        {
            return false; // Retorna falso se houver falha em SDL_RenderCopy
        }

        return true;
    }

private:
    SDL_Surface* Surface;
    SDL_Texture* Texture;
    SDL_Rect sourceball;
    SDL_Rect destinationball;
    int incrementx;
    int incrementy;
};

#endif // BALL_H
