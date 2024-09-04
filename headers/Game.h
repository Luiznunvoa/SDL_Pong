#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Ball.h"
#include "Player.h"
#include <iostream>

class Game
{
public:
    // Construtor
    Game() : window(nullptr),
    renderer(nullptr),
    m_Running(false),
    ball(new Ball()),
    player1(new Player()),
    player2(new Player()) {
        std::fill(std::begin(KEYS), std::end(KEYS), false);
    }

    // Destrutor
    ~Game()
    {
        if (ball)
        {
            ball = nullptr;
            delete ball;
            std::cout << "Ball Unloaded" << std::endl;
        }
        if (player1)
        {
            player1 = nullptr;
            delete player1;
        }
        if (player2)
        {
            player2 = nullptr;
            delete player2;
            std::cout << "Players unloaded" << std::endl;
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
        }// Encerra a SDL corretamente
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
        if (!ball->LoadBall(window, renderer))
        {
            std::cout << "Failed to load ball asset: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        std::cout << "Ball loaded" << std::endl;

        if (!player1->LoadPlayer(window, renderer, true))
        {
            std::cout << "Failed to load player asset: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        if (!player2->LoadPlayer(window, renderer, false))
        {
            std::cout << "Failed to load player asset: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }
        std::cout << "Players loaded" << std::endl;

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
                } break;

                case SDL_KEYDOWN:
                {
                    KEYS[event.key.keysym.sym] = true;
                    std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                } break;

                case SDL_KEYUP:
                {
                    KEYS[event.key.keysym.sym] = false;
                    std::cout << "Key released: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                } break;

                default:
                {
                    //TODO PROCESS UNUSED MESSAGES
                } break;
            }
        }
    }

    // Atualiza o estado do jogo (lógica, física, etc.)
    void update()
    {
        ball->UpdateballPosition(window);
    }

    // Renderiza na janela
    void render()
    {
        // Define o renderizador para preto
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Limpa a tela
        SDL_RenderClear(renderer);

        // Renderiza a ball
        if(!ball->RenderBall(renderer))
        {
            std::cout << "Faild to render ball" << std::endl;
            m_Running = false;
        };
        if(!player1->Renderplayer(renderer))
        {
            std::cout << "Faild to render player" << std::endl;
            m_Running = false;
        };
        if(!player2->Renderplayer(renderer))
        {
            std::cout << "Faild to render player" << std::endl;
            m_Running = false;
        };

        // Exibe o que foi renderizado
        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool KEYS[322];
    bool m_Running;  // variável de controle de execução
    Player* player1;
    Player* player2;
    Ball* ball;
};

#endif // GAME_H
