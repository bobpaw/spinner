#include "main.h"

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
  SDL_Texture * circle = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 2*circle_radius, 2*circle_radius); // Texture, 100p by 100p
  {
    int r = circle_radius;
    int h = 50;
    int k = 50; // (h, k) is circle center point
    int c; // Result of circle, sometimes r*r
    SDL_SetRenderTarget(renderer, circle); // Set renderer target to circle texture
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set clear/draw color to black and clear screen
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // Set draw/clear color to white for drawing points
    int realx; // The x point on the renderer
    int realy; // Y on the renderer
    for (int x = h-r; x < (h+r+1); x++) { // Loop through every x value
      for (int y = k-r; y < (k+r+1); y++) { // Loop through every y value
	c = (x-h) * (x-h) + (y-k) * (y-k);
	if (c == (r * r) ) {
	  SDL_RenderDrawPoint(renderer, x, y); // Create a point where it is exactly
	  /*} else {
	  int dx, dy; // Temporary changes in x or y
	  if (y - dy) // In progress*/
	}
      }
    }
  } // Scope for drawing circle
  SDL_SetRenderTarget(renderer, nullptr); // Reset target to window
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set draw/clear color to black
  SDL_RenderClear(renderer); // Clear screen
  SDL_Rect destrect = {0, 0, 100, 100}; // Circle destination
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
