#include "main.h"

int main (int argc, char * argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    std::cerr << "SDL could not init. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Renderer * renderer = nullptr;
  SDL_Window * window = nullptr;
  window = SDL_CreateWindow("Money", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 400, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cerr << "SDL couldn't create window. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cerr << "SDL couldn't create renderer. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_Delay(1000);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
