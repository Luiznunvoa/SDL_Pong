#include <SDL.h>
#include "headers/Game.h"

int main(int argc, char* argv[])
{
    // Inicializa a variável Global para definir se o programa está rodando
    bool G_Running = false;

    // Inicializa a classe Game
    Game* game = new Game();

    // Inicializa SDL, janela e renderizador
    if(!(G_Running = game->init(SDL_INIT_VIDEO, "MyWindow", 100, 100, 640, 480, SDL_WINDOW_SHOWN)))
    {
        return -1;
    }

    // Renderiza a janela
    game->render();
    /*
        TODO Gameloop:
            while(G_Running)
            {
                game->handleEvents();     // Recebe inputs
                game->update();           // Calcula(física, lógica etc)
                game->render();           // Renderiza
            }
    */

    // Libera a memória alocada para o jogo
    delete game;

    // Finaliza o SDL
    SDL_Quit();

    return 0;
}
