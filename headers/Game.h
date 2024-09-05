#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Ball.h"
#include "Player.h"
#include "Score.h"
#include <iostream>

class Game
{
public:
    // Construtor
    Game():
            windowState{nullptr, nullptr},
            gamestate{false, 0},
            props{new Ball(), new Score()},
            inputState{false, false,
                false, false},
            players{new Player(), new Player()}
    {}

    // Destrutor
    ~Game()
    {
        cleanUp();
    }

    // Define o estado de execução
    void setRunning(bool running)
    {
        gamestate.Running = running;
    }

    // Verifica o estado de execução
    [[nodiscard]] bool isRunning() const
    {
        return gamestate.Running;
    }

    // Inicializa a SDL e configura a janela e o renderizador
    bool init(int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
    {
        if (!initializeSDL(initflag, title, xpos, ypos, width, height, windowflag))
            return false;

        if (!loadAssets())
            return false;

        std::cout << "Initialization Complete" << std::endl;
        return true;
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
                    gamestate.Running = false;
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
        SDL_Rect rect1 = players.player1->UpdatePlayerPosition(windowState.window, inputState.player1UpPressed, inputState.player1DownPressed);
        SDL_Rect rect2 = players.player2->UpdatePlayerPosition(windowState.window, inputState.player2UpPressed, inputState.player2DownPressed);
        gamestate.actualscore += props.ball->UpdateballPosition(windowState.window, rect1, rect2);
    }

    // Renderiza na janela
    void render()
    {
        SDL_SetRenderDrawColor(windowState.renderer, 0, 0, 0, 255);
        SDL_RenderClear(windowState.renderer);

        if (!props.score->RenderScore(windowState.renderer, gamestate.actualscore))
        {
            std::cout << "Failed to render score" << std::endl;
            gamestate.Running = false;
        }

        if (!props.ball->RenderBall(windowState.renderer))
        {
            std::cout << "Failed to render ball" << std::endl;
            gamestate.Running = false;
        }
        if (!players.player1->Renderplayer(windowState.renderer))
        {
            std::cout << "Failed to render player 1" << std::endl;
            gamestate.Running = false;
        }
        if (!players.player2->Renderplayer(windowState.renderer))
        {
            std::cout << "Failed to render player 2" << std::endl;
            gamestate.Running = false;
        }

        SDL_RenderPresent(windowState.renderer);
    }

private:
    // Função para limpar os recursos
    void cleanUp()
    {
        delete props.ball;
        delete players.player1;
        delete players.player2;
        delete props.score;

        if (windowState.renderer)
        {
            SDL_DestroyRenderer(windowState.renderer);
            std::cout << "Renderer Unallocated" << std::endl;
        }
        if (windowState.window)
        {
            SDL_DestroyWindow(windowState.window);
            std::cout << "Window Unallocated" << std::endl;
        }
        SDL_Quit();
    }

    // Função para inicializar SDL e criar a janela e o renderizador
    bool initializeSDL(int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
    {
        if (SDL_Init(initflag) < 0)
        {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        windowState.window = SDL_CreateWindow(title, xpos, ypos, width, height, windowflag);
        if (windowState.window == nullptr)
        {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        windowState.renderer = SDL_CreateRenderer(windowState.window, -1, 0);
        if (windowState.renderer == nullptr)
        {
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(windowState.window);
            SDL_Quit();
            return false;
        }

        return true;
    }

    // Lida com eventos de entrada
    void handleKeyInput(SDL_Event event, int condition)
    {
        const char* array[] = {"PRESSED", "RELEASED"};

        switch (event.key.keysym.sym)
        {
            case SDLK_w:
                inputState.player1UpPressed = (condition == 0);
            std::cout << "Player 1 UP " << array[condition] << std::endl;
            break;
            case SDLK_s:
                inputState.player1DownPressed = (condition == 0);
            std::cout << "Player 1 DOWN " << array[condition] << std::endl;
            break;
            case SDLK_UP:
                inputState.player2UpPressed = (condition == 0);
            std::cout << "Player 2 UP " << array[condition] << std::endl;
            break;
            case SDLK_DOWN:
                inputState.player2DownPressed = (condition == 0);
            std::cout << "Player 2 DOWN " << array[condition] << std::endl;
            break;
            default:
                std::cout << "Unknown key " << array[condition] << std::endl;
        }
    }

    // Função para carregar os assets (bola, jogadores e placar)
    bool loadAssets()
    {
        if (!props.ball->LoadBall(windowState.window, windowState.renderer) ||
            !players.player1->LoadPlayer(windowState.window, windowState.renderer, true) ||
            !players.player2->LoadPlayer(windowState.window, windowState.renderer, false) ||
            !props.score->LoadScore(windowState.window, windowState.renderer
            ))
        {
            SDL_DestroyRenderer(windowState.renderer);
            SDL_DestroyWindow(windowState.window);
            SDL_Quit();
            return false;
        }
        return true;
    }

    //GLOBALS//

    // Estrutura para o estado dos jogadores
    struct PlayerState
    {
        Player* player1;
        Player* player2;
    };PlayerState players;

    // Estrutura para o estado de entrada
    struct InputState
    {
        bool player1UpPressed;
        bool player1DownPressed;
        bool player2UpPressed;
        bool player2DownPressed;
    };InputState inputState;

    struct WindowState
    {
        SDL_Window* window;
        SDL_Renderer* renderer;
    };WindowState windowState;

    struct GameState
    {
        bool Running;
        int actualscore;
    };GameState gamestate;

    struct Props
    {
        Ball* ball;
        Score* score;
    };Props props;

};

#endif // GAME_H
