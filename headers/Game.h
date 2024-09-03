#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <iostream>

class Game
{
public:
    // Construtor
    Game() : window(nullptr), renderer(nullptr), G_Running(true) {}

    // Destrutor
    ~Game()
    {
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
        if (window)
        {
            SDL_DestroyWindow(window);
        }
    }

    bool G_Running;

    // Inicializa a SDL e configura a janela e o renderizador
    bool init(int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
    {
        // Inicialização do SDL
        if (SDL_Init(initflag) < 0) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // Criação da janela
        window = SDL_CreateWindow(title, xpos, ypos, width, height, windowflag);
        if (window == nullptr)
        {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        // Criação do renderizador
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == nullptr)
        {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        return true;
    }

    // Lida com eventos de entrada
    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    std::cout << "SDL_QUIT" << std::endl;
                    G_Running = false;
                }
                case SDL_DISPLAYEVENT:
                {
                    std::cout << "SDL_DISPLAYEVENT " << std::endl;
                }
                case SDL_WINDOWEVENT:
                {
                    std::cout << "SDL_WINDOWEVENT " << std::endl;
                }

            }
            // Outros eventos como teclas pressionadas podem ser adicionados aqui

        }
    }

    // Atualiza o estado do jogo (lógica, física, etc.)
    void update()
    {
        // TODO Lógica do Jogo
    }

    // Renderiza na janela
    void render()
    {
        // Define o renderizador para preto
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Limpa a tela
        SDL_RenderClear(renderer);

        // TODO MOSTRAR CONTEÚDO

        // Exibe o que foi renderizado
        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;// Indica se o jogo está rodando
};

#endif //GAME_H
