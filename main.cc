#include "main.h"
#include <cmath>

int drawCircle (SDL_Renderer * renderer, int h, int k, int r) {
  int x = r-1;
  int y = 0;
  int dx = 1;
  int dy = 1;
  int err = dx - r * 2;

  while (x >= y) {
    SDL_RenderDrawPoint(renderer, h + x, k + y);
    SDL_RenderDrawPoint(renderer, h + y, k + x);
    SDL_RenderDrawPoint(renderer, h - y, k + x);
    SDL_RenderDrawPoint(renderer, h - x, k + y);
    SDL_RenderDrawPoint(renderer, h - x, k - y);
    SDL_RenderDrawPoint(renderer, h - y, k - x);
    SDL_RenderDrawPoint(renderer, h + y, k - x);
    SDL_RenderDrawPoint(renderer, h + x, k - y);
    if (err <= 0) {
      y++;
      err += dy;
      dy += 2;
    } else {
      x--;
      dx += 2;
      err += -2 * r + dx;
    }
  }
  return 0;
}

int main (int argc, char * argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    std::cerr << "SDL could not init. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Renderer * renderer = nullptr;
  SDL_Window * window = nullptr;
  window = SDL_CreateWindow("Money", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 400, SDL_WINDOW_SHOWN); // 640x400 window with title set to 'Money'
  if (window == nullptr) {
    std::cerr << "SDL couldn't create window. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
  if (renderer == nullptr) {
    std::cerr << "SDL couldn't create renderer. SDL_error: " << SDL_GetError() << std::endl;
    return -1;
  }
  int circle_radius = 50;
  SDL_Point circle_center;
  circle_center.x = 50;
  circle_center.y = 50;
  SDL_Texture * circle = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 2*circle_radius, 2*circle_radius); // Texture, 100p by 100p
  SDL_SetRenderTarget(renderer, circle); // Set renderer target to circle texture
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set clear/draw color to black and clear screen
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // Set draw/clear color to white for drawing points
  drawCircle(renderer, circle_center.x, circle_center.y, circle_radius); // Draw circle at (50, 50)
  SDL_Point lines[3];
  int percent_chance = 30;
  const double pi = std::atan(1)*4;
  lines[0].x = 100;
  lines[0].y = 50;
  lines[1].x = 50;
  lines[1].y = 50;
  lines[2].x = (std::cos(percent_chance*(2*pi/100)) * circle_radius) + circle_center.x;
  lines[2].y = (std::sin(percent_chance*(2*pi/100)) * circle_radius) + circle_center.y;
  SDL_RenderDrawLines(renderer, lines, 3);
  SDL_SetRenderTarget(renderer, nullptr); // Reset target to window
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set draw/clear color to black
  SDL_RenderClear(renderer); // Clear screen
  SDL_Rect destrect = {0, 0, 2*circle_radius, 2*circle_radius}; // Circle destination
  SDL_RenderCopy(renderer, circle, nullptr, &destrect); // Copy circle to renderer
  SDL_RenderPresent(renderer);
  SDL_Event e;
  do {
    SDL_Delay(20);
    SDL_PollEvent(&e);
  } while (e.type != SDL_QUIT); // Wait until big X (at top of window) is clicked
  SDL_DestroyTexture(circle); // Destroy circle texture
  SDL_DestroyRenderer(renderer); // Kill renderer
  SDL_DestroyWindow(window); // Kill Window
  SDL_Quit(); // Deallocate SDL resources
  return 0;
}
