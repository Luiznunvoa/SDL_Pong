#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    /*
        SDL_INIT_HAPTIC Force feedback subsystem
        SDL_INIT_AUDIO Audio subsystem
        SDL_INIT_VIDEO Video subsystem
        SDL_INIT_TIMER Timer subsystem
        SDL_INIT_JOYSTICK Joystick subsystem
        SDL_INIT_EVERYTHING All subsystems
        SDL_INIT_NOPARACHUTE Don't catch fatal signal
     */

    // Inicialização do SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Definição do Ponteiro Handle da janela
    SDL_Window *window = SDL_CreateWindow("Hello SDL", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Definição do ponteiro para o renderizador da janela
    SDL_Renderer* g_pRenderer = SDL_CreateRenderer(window, -1, 0);
    if (g_pRenderer == nullptr) { // Adicione uma verificação para SDL_CreateRenderer
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Define o Renderizador para preto
    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

    // Manda o renderizador pintar a janela toda
    SDL_RenderClear(g_pRenderer);

    // Mostra oque foi renderizado
    SDL_RenderPresent(g_pRenderer);

    // Espera um cadinho
    SDL_Delay(2000);

    // Desaloca o handle da janela e do renderizador
    SDL_DestroyRenderer(g_pRenderer);
    SDL_DestroyWindow(window);

    // Finaliza o SDL
    SDL_Quit();

    return 0;
}
