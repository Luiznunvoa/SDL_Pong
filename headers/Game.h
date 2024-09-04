#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <iostream>

class Game
{
public:
    // Construtor
    Game() : window(nullptr), renderer(nullptr), pTempSurface(nullptr), m_pTexture(nullptr) {}

    // Destrutor
    ~Game()
    {
        if (m_pTexture)
        {
            std::cout << "Texture Unloaded" << std::endl;
            SDL_DestroyTexture(m_pTexture);
        }
        if (renderer)
        {
            std::cout << "Render Unallocated" << std::endl;
            SDL_DestroyRenderer(renderer);
        }
        if (window)
        {
            std::cout << "Window Unallocated" << std::endl;
            SDL_DestroyWindow(window);
        }
        if (pTempSurface)
        {
            std::cout << "Textures Unloaded" << std::endl;
            SDL_FreeSurface(pTempSurface);
        }
    }

    bool G_Running = true;

    // Inicializa a SDL e configura a janela e o renderizador
    bool init(int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
    {
        // Inicialização do SDL
        if (SDL_Init(initflag) < 0) {
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            G_Running = false;
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
            SDL_DestroyWindow(window); // Libera o recurso da janela
            SDL_Quit();
            return false;
        }
        std::cout << "Renderer Created" << std::endl;

        // Carregamento de superfície temporária
        pTempSurface = SDL_LoadBMP("../assets/rider.bmp");
        if (pTempSurface == nullptr)
        {
            std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer); // Libera o recurso do renderizador
            SDL_DestroyWindow(window); // Libera o recurso da janela
            SDL_Quit();
            return false;
        }
        std::cout << "Surface Loaded" << std::endl;

        std::cout << "Initialization Complete" << std::endl;
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
                    std::cout << "SDL_QUIT" << std::endl;
                    G_Running = false;
                    break;
                case SDL_DISPLAYEVENT:
                    std::cout << "SDL_DISPLAYEVENT" << std::endl;
                    break;
                case SDL_WINDOWEVENT:
                    std::cout << "SDL_WINDOWEVENT" << std::endl;
                    break;
                // Outros eventos como teclas pressionadas podem ser adicionados aqui
            }
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
    SDL_Renderer* renderer;
    SDL_Surface* pTempSurface;
    SDL_Texture* m_pTexture; // variável SDL_Texture
};

#endif //GAME_H
