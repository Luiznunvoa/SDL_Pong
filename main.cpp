#include <SDL.h>
#include "headers/Game.h"

// Define o intervalo de ticks (tempo entre frames) para 30ms (~33 FPS)
#define TICK_INTERVAL 30

// Função para calcular o tempo restante até o próximo frame
static Uint32 time_left(Uint32 next_time)
{
    Uint32 now = SDL_GetTicks();  // Pega o tempo atual
    return (next_time <= now) ? 0 : next_time - now;
}

int main(int argc, char* argv[])
{
    // Inicializa o gerenciador do jogo
    Game* gameManager = new Game();

    // Inicializa o SDL, cria a janela e o renderizador
    if (!gameManager->init(SDL_INIT_VIDEO, "MyWindow",
        100, 50, 1280, 960,
        SDL_WINDOW_SHOWN
        ))
    {
        // Se houver falha na inicialização, retorna com erro
        return -1;
    }

    gameManager->setRunning(true);  // Inicia o loop do jogo

    Uint32 next_time = SDL_GetTicks() + TICK_INTERVAL;  // Tempo do próximo frame

    // Loop principal do jogo
    while (gameManager->isRunning())
    {
        gameManager->handleEvents();  // Processa os eventos de input (teclado, mouse etc)
        gameManager->update();        // Atualiza a lógica do jogo
        gameManager->render();        // Renderiza os gráficos

        // Aguarda o tempo necessário para manter o FPS em ~33
        SDL_Delay(time_left(next_time));
        next_time += TICK_INTERVAL;  // Calcula o tempo para o próximo frame
    }

    // Libera a memória usada pelo gerenciador do jogo
    delete gameManager;
    std::cout << "Game Finalized" << std::endl;

    // Finaliza a SDL
    SDL_Quit();
    std::cout << "SDL Finalized" << std::endl;

    return 0;
}