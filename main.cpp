#include <SDL.h>
#include "headers/Game.h"

int main(int argc, char* argv[])
{
    // Inicializa a classe Game
    Game* gameManager = new Game();

    // Inicializa SDL, janela e renderizador
    if (!gameManager->init(SDL_INIT_VIDEO, "MyWindow", 100, 100, 640, 480, SDL_WINDOW_SHOWN))
    {
        return -1;
    }

    // Loop principal do jogo
    while (gameManager->G_Running)
    {
        gameManager->handleEvents();  // Recebe inputs
        gameManager->update();        // Calcula(física, lógica etc)
        gameManager->render();        // Renderiza
    }

    // Libera a memória alocada para o jogo
    delete gameManager;

    // Finaliza o SDL
    SDL_Quit();

    return 0;
}
