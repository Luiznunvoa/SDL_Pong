//
// Created by Luiz on 04/09/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>


class Player
{
public:
    Player() : Surface(nullptr),
    Texture(nullptr),
    sourceplayer{0, 0, 0, 0},
    destinationplayer{0, 0, 0, 0},
    currentdirection(0){}

    ~Player()
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

    bool LoadPlayer(SDL_Window* window, SDL_Renderer* renderer, bool p1) {
        Surface = SDL_LoadBMP("../assets/Player.bmp");
        if (Surface == nullptr)
        {
            return false;
        }

        Texture = SDL_CreateTextureFromSurface(renderer, Surface);

        if (Texture == nullptr)
        {
            return false;
        }

        // Defina o tamanho do sourceplayer após carregar a textura
        SDL_QueryTexture(Texture, nullptr, nullptr, &sourceplayer.w, &sourceplayer.h);
        SDL_GetWindowSize(window, &destinationplayer.x, &destinationplayer.y);
        if(p1)
        {
            destinationplayer.x = 50;
        }
        else
        {
            destinationplayer.x -= 100;
        }

        destinationplayer.y /= 4;

        return true;
    }

    void UpdatePlayerPosition(SDL_Window* window, bool Up, bool Down)
    {
        // Definir a direção atual com base nas teclas pressionadas
        if (Up)
        {
            currentdirection = -5; // Mover para cima
        }
        else if (Down)
        {
            currentdirection = 5;  // Mover para baixo
        }
        else
        {
            currentdirection = 0;  // Parado se nenhuma tecla pressionada
        }

        int windowWidth, windowHeight;

        // Obter o tamanho da janela
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // Verifica se o jogador não vai ultrapassar as bordas da janela
        if ((currentdirection < 0 && destinationplayer.y > 0) ||  // Movendo para cima, mas não ultrapassa o topo
            (currentdirection > 0 && (destinationplayer.y + destinationplayer.h) < windowHeight))  // Movendo para baixo, mas não ultrapassa a base
        {
            destinationplayer.y += currentdirection;
        }
    }


    bool Renderplayer(SDL_Renderer* renderer)
    {
        if (Texture == nullptr)
        {
            return false; // Garantir que não tentamos renderizar uma textura nula
        }

        // Define a posição e o tamanho do destinationplayer
        destinationplayer.w = sourceplayer.w;
        destinationplayer.h = sourceplayer.h;

        if (SDL_RenderCopy(renderer, Texture, &sourceplayer, &destinationplayer) != 0)
        {
            return false; // Retorna falso se houver falha em SDL_RenderCopy
        }

        return true;
    }

private:
    SDL_Surface* Surface;
    SDL_Texture* Texture;
    SDL_Rect sourceplayer;
    SDL_Rect destinationplayer;
    int currentdirection;
};

#endif //PLAYER_H
