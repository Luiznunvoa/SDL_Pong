#include <SDL.h>
#include "headers/Game.h"

#define TICK_INTERVAL    30

static Uint32 next_time = SDL_GetTicks() + TICK_INTERVAL;;

static Uint32 time_left()
{
    Uint32 now = SDL_GetTicks();
    if(next_time <= now)
    {
        return 0;
    }
    return next_time - now;
}

int main(int argc, char* argv[])
{
    // Inicializa a classe Game
    Game* gameManager = new Game();

    // Inicializa SDL, janela e renderizador e verifica a inicialização
    if (!gameManager->init(SDL_INIT_VIDEO, "MyWindow", 100, 100, 640, 480, SDL_WINDOW_SHOWN))
    {
        return -1;
    }

    gameManager->setRunning(true);

    // Loop principal do jogo
    while (gameManager->isRunning())
    {
        gameManager->handleEvents();  // Recebe inputs
        gameManager->update();        // Calcula(física, lógica etc)
        gameManager->render();        // Renderiza

        SDL_Delay(time_left());   // Define um tempo de Delay para manter o FPS em, no máximo, 30
        next_time += TICK_INTERVAL;
    }

    // Libera a memória alocada para o jogo
    delete gameManager;
    // Finaliza o SDL
    SDL_Quit();

    return 0;
}
