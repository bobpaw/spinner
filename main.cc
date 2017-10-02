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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
  if (renderer == nullptr) {
    std::cerr << "SDL couldn't create renderer. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Texture * circle = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 100, 100);
  int circle_radius = 50;
  int h = 50;
  int k = 50; // (h, k) is circle center point
  SDL_SetRenderTarget(renderer, circle);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  for (int x = 0, y = 0; x < 100 && y < 100; x++, y++) {
    int realx = x - 50;
    int realy = y - 50;
    if ((realx-h) * (realx - h) + (realy-k) * (realy-k) == (circle_radius * circle_radius) ) SDL_RenderDrawPoint(renderer, x, y);
  }
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderClear(renderer);
  SDL_Rect destrect = {0, 0, 100, 100};
  SDL_RenderCopy(renderer, circle, nullptr, &destrect);
  SDL_RenderPresent(renderer);
  SDL_Delay(1000);
  SDL_DestroyTexture(circle);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
