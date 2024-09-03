#include <SDL.h>
#include <iostream>

// Inicializa a SDL e configura a janela e o renderizador
static bool init(SDL_Window** window, SDL_Renderer** renderer, int initflag, const char* title, int xpos, int ypos, int width, int height, int windowflag)
{
    // Inicialização do SDL
    if (SDL_Init(initflag) < 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Definição do Ponteiro Handle da janela
    *window = SDL_CreateWindow(title, xpos, ypos, width, height, windowflag);
    if (*window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Definição do ponteiro para o renderizador da janela
    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (*renderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

static void render(SDL_Renderer* renderer)
{
    // Define o Renderizador para preto
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Manda o renderizador pintar a janela toda
    SDL_RenderClear(renderer);

    // Mostra o que foi renderizado
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    // Definição da variável para definir se o jogo esta rodando
    bool Running = false;

    // Definição dos handlres da janela e do renderizador
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // Inicializa SDL, janela e renderizador
    if(!(Running = init(&window, &renderer, SDL_INIT_VIDEO, "MyWindow", 100, 100, 640, 480, SDL_WINDOW_SHOWN)))
    {
        return -1;
    }

    render(renderer);

    /* TODO (Gameloop)
            while(Running)
        {
            handleEvents();     // Recebe inputs
            update();           // Calcula(física, lógica etc)
            render(renderer);   // Renderiza
        }
    */

    // Espera um cadinho
    SDL_Delay(2000);

    // Desaloca o handle da janela e do renderizador
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Finaliza o SDL
    SDL_Quit();

    return 0;
}
