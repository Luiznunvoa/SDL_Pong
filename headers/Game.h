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
    player2(new Player()),
    player1UpPressed(false),
    player1DownPressed(false),
    player2UpPressed(false),
    player2DownPressed(false) {}

    // Destrutor
    ~Game()
    {
        if (ball)
        {
            delete ball;
            ball = nullptr;
            std::cout << "Ball Unloaded" << std::endl;
        }
        if (player1)
        {
            delete player1;
            player1 = nullptr;
        }
        if (player2)
        {
            delete player2;
            player2 = nullptr;
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
        }
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

    // Lida com eventos de entrada
    void handleKeyInput(SDL_Event event, int condition)
    {
        const char* array[] = {"PRESSED", "RELEASED"};

        switch (event.key.keysym.sym)
        {
            // Player 1 (W e S)
            case SDLK_w:
                player1UpPressed = (condition == 0); // Pressionado ou solto
                std::cout << "Player 1 UP " << array[condition] << std::endl;
                break;
            case SDLK_s:
                player1DownPressed = (condition == 0);
                std::cout << "Player 1 DOWN " << array[condition] << std::endl;
                break;

            // Player 2 (UP e DOWN)
            case SDLK_UP:
                player2UpPressed = (condition == 0);
                std::cout << "Player 2 UP " << array[condition] << std::endl;
                break;
            case SDLK_DOWN:
                player2DownPressed = (condition == 0);
                std::cout << "Player 2 DOWN " << array[condition] << std::endl;
                break;

            default:
                std::cout << "Unknown key " << array[condition] << std::endl;
        }
    }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    std::cout << "Message received: SDL_QUIT" << std::endl;
                    m_Running = false;
                    break;

                case SDL_KEYDOWN:
                    handleKeyInput(event, 0);
                    break;

                case SDL_KEYUP:
                    handleKeyInput(event, 1);
                    break;

                default:
                    break;
            }
        }
    }

    // Atualiza o estado do jogo (lógica, física, etc.)
    void update()
    {
        SDL_Rect rect1 = player1->UpdatePlayerPosition(window, player1UpPressed, player1DownPressed);
        SDL_Rect rect2 = player2->UpdatePlayerPosition(window, player2UpPressed, player2DownPressed);
        ball->UpdateballPosition(window, rect1, rect2);
        // Aqui você pode usar as variáveis player1UpPressed, player1DownPressed, player2UpPressed, player2DownPressed
        // para movimentar os jogadores
    }

    // Renderiza na janela
    void render()
    {
        // Define o renderizador para preto
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Limpa a tela
        SDL_RenderClear(renderer);

        // Renderiza a bola e os jogadores
        if (!ball->RenderBall(renderer))
        {
            std::cout << "Failed to render ball" << std::endl;
            m_Running = false;
        }
        if (!player1->Renderplayer(renderer))
        {
            std::cout << "Failed to render player 1" << std::endl;
            m_Running = false;
        }
        if (!player2->Renderplayer(renderer))
        {
            std::cout << "Failed to render player 2" << std::endl;
            m_Running = false;
        }

        // Exibe o que foi renderizado
        SDL_RenderPresent(renderer);
    }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool m_Running;  // variável de controle de execução
    Player* player1;
    Player* player2;
    Ball* ball;

    // Variáveis para armazenar o estado das teclas
    bool player1UpPressed;
    bool player1DownPressed;
    bool player2UpPressed;
    bool player2DownPressed;
};

#endif // GAME_H
