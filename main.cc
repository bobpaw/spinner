#include "main.h"
#include <cmath>
#include <random>

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
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cerr << "SDL_Image couldn't init!" << std::endl;
    return -1;
  }
  if (TTF_Init() == -1) {
    std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
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
  TTF_Font * font = TTF_OpenFont("FreeMono.ttf", 16);
  if (font == nullptr) {
    std::cerr << "Failed to load FreeMono font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    return -1;
  }
  std::random_device random;
  float percent_chance = 25;
  int circle_radius = 100;
  SDL_Event e;
  int sdl_angle = 0;
  SDL_Point rotate_point;
  int x = 0;
  int add = 20;
  SDL_Color textcolor = {255, 255, 255};
  SDL_Texture * money_texture = nullptr;
  SDL_Rect money_dest_rect = {0, circle_radius*2, 0, 0};
  SDL_Texture * circle = nullptr;
  SDL_Texture * spinner = nullptr;
  SDL_Rect spinner_rect;
  SDL_Rect spinner_dest_rect;
  SDL_Point lines[3];
  const double pi = std::atan(1)*4;
  {
    SDL_Surface * temp_surface = nullptr;
    temp_surface = IMG_Load("protospinner.png");
    if (temp_surface == nullptr) {
      std::cerr << "Unable to load image protospinner.png. SDL_image Error: " << IMG_GetError() << std::endl;
      return -1;
    }
    spinner = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if (spinner == nullptr) {
      std::cerr << "Unable to create texture protospinner.png. SDL Error: " << SDL_GetError() << std::endl;
      return -1;
    }
    spinner_rect.w = temp_surface->w;
    spinner_rect.h = temp_surface->h;
    SDL_FreeSurface(temp_surface);
    temp_surface = TTF_RenderText_Solid(font, "Money: ", textcolor);
    if (temp_surface == nullptr) {
      std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
      return -1;
    }
    money_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if (money_texture == nullptr) {
      std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError << std::endl;
      return -1;
    }
    money_dest_rect.w = temp_surface->w;
    money_dest_rect.h = temp_surface->h;
    SDL_FreeSurface(temp_surface);
  }
  SDL_Point circle_center;
  circle_center.x = circle_radius;
  circle_center.y = circle_radius;
  spinner_rect.x = 0;
  spinner_rect.y = 0;
  spinner_dest_rect = spinner_rect;
  spinner_dest_rect.h = circle_radius*90/100;
  spinner_dest_rect.w = (90*circle_radius/100)*139/313;
  spinner_dest_rect.x = circle_center.x - spinner_dest_rect.w/2;
  spinner_dest_rect.y = circle_center.y - spinner_dest_rect.h;
  {
  circle = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 2*circle_radius, 2*circle_radius); // Texture, 100p by 100p
  SDL_SetRenderTarget(renderer, circle); // Set renderer target to circle texture
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set clear/draw color to black and clear screen
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // Set draw/clear color to white for drawing points
  drawCircle(renderer, circle_center.x, circle_center.y, circle_radius); // Draw circle at (50, 50)
  lines[0].x = 2*circle_radius;
  lines[0].y = circle_radius;
  lines[1].x = circle_radius;
  lines[1].y = circle_radius;
  lines[2].x = (std::cos(percent_chance*(2*pi/100)) * circle_radius) + circle_center.x;
  lines[2].y = (std::sin(percent_chance*(2*pi/100)) * circle_radius) + circle_center.y;
  SDL_RenderDrawLines(renderer, lines, 3);
  SDL_SetRenderTarget(renderer, nullptr); // Reset target to window
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set draw/clear color to black
  }
  SDL_RenderClear(renderer); // Clear screen
  SDL_Rect circle_dest_rect = {0, 0, 2*circle_radius, 2*circle_radius}; // Circle destination
  SDL_RenderCopy(renderer, circle, nullptr, &circle_dest_rect); // Copy circle to renderer
  SDL_RenderPresent(renderer);
  rotate_point.x = spinner_dest_rect.w/2;
  rotate_point.y = spinner_dest_rect.h;
  int rand_val = random() % 1000;
  do {
    SDL_Delay(1);
    sdl_angle+=add;
    x++;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, money_texture, nullptr, &money_dest_rect);
    SDL_RenderCopy(renderer, circle, nullptr, &circle_dest_rect);
    SDL_RenderCopyEx(renderer, spinner, &spinner_rect, &spinner_dest_rect, (double) sdl_angle, &rotate_point, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) goto end;
  } while (x < rand_val);
  x = 0;
  do {
    if (x < 100) {
      x++;
      SDL_Delay(x);
    } else break;
    if (x%5==0&&add>1) add--;
    sdl_angle+=add;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, money_texture, nullptr, &money_dest_rect);
    SDL_RenderCopy(renderer, circle, nullptr, &circle_dest_rect);
    SDL_RenderCopyEx(renderer, spinner, &spinner_rect, &spinner_dest_rect, (double) sdl_angle, &rotate_point, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) goto end;
  } while (x != 100); // Wait until big X (at top of window) is clicked
  do {
    SDL_Delay(20);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&e);
  } while (e.type != SDL_QUIT);
 end:
  SDL_DestroyTexture(circle); // Destroy circle texture
  SDL_DestroyTexture(spinner);
  SDL_DestroyTexture(money_texture);
  SDL_DestroyRenderer(renderer); // Kill renderer
  SDL_DestroyWindow(window); // Kill Window
  TTF_Quit();
  IMG_Quit();
  SDL_Quit(); // Deallocate SDL resources
  return 0;
}
