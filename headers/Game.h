#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Crate.h"
#include <iostream>

class Game
{
public:
    // Construtor
    Game() : window(nullptr), renderer(nullptr), m_Running(false), crate(new Crate()) {}

    // Destrutor
    ~Game()
    {
        if (crate)
        {

            delete crate;
            crate = nullptr;
            std::cout << "Assets Unloaded" << std::endl;
        }
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
            std::cout << "Renderer Unallocated" << std::endl;
        }
        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
            std::cout << "Window Unallocated" << std::endl;
        }
        SDL_Quit(); // Encerra a SDL corretamente
    }

    // Inicializa a SDL e configura a janela e o renderizador
    bool init(int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
    {
        // Inicialização do SDL
        if (SDL_Init(initflag) < 0)
        {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }
        std::cout << "SDL Initialized" << std::endl;

        // Criação da janela
        window = SDL_CreateWindow(title, xpos, ypos, width, height, windowflag);
        if (window == nullptr)
        {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        std::cout << "Window Created" << std::endl;

        // Criação do renderizador
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == nullptr)
        {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        std::cout << "Renderer Created" << std::endl;

        // Carrega os assets
        if (!crate->LoadCrate(renderer))
        {
            std::cout << "Failed to load crate asset: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        std::cout << "Assets Loaded" << std::endl;
        std::cout << "Initialization Complete" << std::endl;
        return true;
    }

    // Define o estado de execução
    void setRunning(bool running)
    {
        m_Running = running;
    }

    // Verifica o estado de execução
    [[nodiscard]] bool isRunning() const
    {
        return m_Running;
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
                    std::cout << "Message received: SDL_QUIT" << std::endl;
                    m_Running = false;
                    break;
                }
                default:
                {
                    std::cout << "Message not processed" << std::endl;
                }
                // Outros eventos como teclas pressionadas podem ser adicionados aqui
            }
        }
    }

    // Atualiza o estado do jogo (lógica, física, etc.)
    void update()
    {
        crate->UpdateRectanglePosition(window);
    }

    // Renderiza na janela
    void render()
    {
        // Define o renderizador para preto
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Limpa a tela
        SDL_RenderClear(renderer);

        // Renderiza a crate
        if(!crate->RenderCrate(renderer))
        {
            std::cout << "Faild to render Crate" << std::endl;
            m_Running = false;
        };

        // Exibe o que foi renderizado
        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool m_Running;  // variável de controle de execução
    Crate* crate;
};

#endif // GAME_H
