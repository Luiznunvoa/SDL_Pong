#ifndef BALL_H
#define BALL_H

#include <SDL.h>

// Classe do retangulo
class Ball
{
public:
    Ball() : Surface(nullptr),
    Texture(nullptr),
    sourceball{0, 0, 0, 0},
    destinationball{0, 0, 0, 0},
    incrementx(5), incrementy(5) {}

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

    // Verifica a colisão da bola com o jogador
    bool CheckCollisionWithPlayer(SDL_Rect playerRect)
    {
        // Verifica se há sobreposição de retângulos (detecção de colisão)
        if (destinationball.x < playerRect.x + playerRect.w &&
            destinationball.x + destinationball.w > playerRect.x &&
            destinationball.y < playerRect.y + playerRect.h &&
            destinationball.y + destinationball.h > playerRect.y)
        {
            return true; // Colisão detectada
        }
        return false;
    }

    //Altera a posição do retangulo
    void UpdateballPosition(SDL_Window* window, SDL_Rect player1Rect, SDL_Rect player2Rect)
    {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        // Verificar colisão com as bordas da tela
        if ((destinationball.x + destinationball.w == width && incrementx > 0) ||
            (destinationball.x == 0 && incrementx < 0))
        {
            incrementx = -incrementx;  // Reverter a direção no eixo X
            destinationball.x = (width / 2);
            destinationball.y = (height / 2);
        }

        if ((destinationball.y + destinationball.h == height && incrementy > 0) ||
            (destinationball.y == 0 && incrementy < 0))
        {
            incrementy = -incrementy;  // Reverter a direção no eixo Y
        }

        // Verificar colisão com os jogadores
        if (CheckCollisionWithPlayer(player1Rect) || CheckCollisionWithPlayer(player2Rect))
        {
            incrementx = -incrementx;  // Reverter a direção ao colidir com os jogadores
        }

        // Atualizar a posição da bola
        destinationball.x += incrementx;
        destinationball.y += incrementy;
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