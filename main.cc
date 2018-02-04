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
  // Initialize modules
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
  // Create module objects
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

  // Variables
  const double pi = std::atan(1)*4;
  std::random_device random;
  float percent_chance = 25;
  int circle_radius = 100;
  SDL_Event e;
  bool quit_pressed = false;
  int money = 100;
  std::string money_string = std::to_string(money);

  // SDL_ttf variables
  SDL_Color textcolor = {255, 255, 255};
  SDL_Texture * money_texture = nullptr;
  SDL_Texture * nums_texture = nullptr;
  SDL_Rect money_dest_rect = {0, circle_radius*2, 0, 0};
  SDL_Rect nums_source_rect[10];
  SDL_Rect num_dest_rect = {0, 0, 0, 0};

  // Textures and Textures information
  SDL_Texture * circle = nullptr;
  SDL_Point circle_center;
  SDL_Rect circle_dest_rect;
  SDL_Texture * spinner = nullptr;
  SDL_Rect spinner_rect;
  SDL_Rect spinner_dest_rect;

  // Texture information assignments
  circle_center.x = circle_radius;
  circle_center.y = circle_radius;
  circle_dest_rect.x = 0;
  circle_dest_rect.y = 0;
  circle_dest_rect.h = 2 * circle_radius;
  circle_dest_rect.w = 2 * circle_radius;
  spinner_rect.x = 0;
  spinner_rect.y = 0;
  spinner_dest_rect = spinner_rect;
  spinner_dest_rect.h = circle_radius*90/100;
  spinner_dest_rect.w = (90*circle_radius/100)*139/313;
  spinner_dest_rect.x = circle_center.x - spinner_dest_rect.w/2;
  spinner_dest_rect.y = circle_center.y - spinner_dest_rect.h;

  // Rotation variables
  int rand_val = 0;
  int sdl_angle = 0;
  int delay_time = 0;
  int add = 20;
  SDL_Point rotate_point;
  rotate_point.x = spinner_dest_rect.w/2;
  rotate_point.y = spinner_dest_rect.h;
  
  // Load/Create Textures
  circle = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 2*circle_radius, 2*circle_radius);
  { // Load spinner from file
    SDL_Surface * temp_surface = nullptr;

    // Load surface from PNG
    temp_surface = IMG_Load("protospinner.png");
    if (temp_surface == nullptr) {
      std::cerr << "Unable to load image protospinner.png. SDL_image Error: " << IMG_GetError() << std::endl;
      return -1;
    }

    // Optimize surface into hardware data
    spinner = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if (spinner == nullptr) {
      std::cerr << "Unable to create texture protospinner.png. SDL Error: " << SDL_GetError() << std::endl;
      return -1;
    }

    // Extract dimensions
    spinner_rect.w = temp_surface->w;
    spinner_rect.h = temp_surface->h;
    SDL_FreeSurface(temp_surface);

    // Create 'Money: ' text
    temp_surface = TTF_RenderText_Solid(font, "Money: ", textcolor);
    if (temp_surface == nullptr) {
      std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
      return -1;
    }

    // Optimize text
    money_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if (money_texture == nullptr) {
      std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError << std::endl;
      return -1;
    }

    // Extract dimensions
    money_dest_rect.w = temp_surface->w;
    money_dest_rect.h = temp_surface->h;
    SDL_FreeSurface(temp_surface);

    // Create '0123456789' spritemap
    temp_surface = TTF_RenderText_Solid(font, "0123456789", textcolor);
    if (temp_surface == nullptr) {
      std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
      return -1;
    }

    // Optimize surface
    nums_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if (nums_texture == nullptr) {
      std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError << std::endl;
      return -1;
    }

    // Extract dimensions
    for (int i = 0; i < 10; i++) {
      nums_source_rect[i].w = temp_surface->w / 10;
      nums_source_rect[i].h = temp_surface->h;
      nums_source_rect[i].x = nums_source_rect[i].w * i;
      nums_source_rect[i].y = 0;
    }
    num_dest_rect.w = nums_source_rect[0].w;
    num_dest_rect.h = nums_source_rect[0].h;
    SDL_FreeSurface(temp_surface);
    
  }

  // Draw circle and lines
  {
    SDL_SetRenderTarget(renderer, circle); // Set renderer target to circle texture
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Set clear/draw color to black and clear screen
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // Set draw/clear color to white for drawing points
    drawCircle(renderer, circle_center.x, circle_center.y, circle_radius); // Draw circle at (50, 50)
    SDL_Point lines[3];
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

  // Pre-loop
  SDL_RenderClear(renderer); // Clear screen
  SDL_RenderCopy(renderer, circle, nullptr, &circle_dest_rect); // Copy circle to renderer
  SDL_RenderPresent(renderer);

  // Random spin time
  rand_val = random() % 1000;
  while (delay_time < rand_val && quit_pressed == false) {
    SDL_Delay(1);
    sdl_angle += add;
    delay_time++;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, money_texture, nullptr, &money_dest_rect);
    num_dest_rect.y = money_dest_rect.y;
    money_string = std::to_string(money);
    num_dest_rect.x = money_dest_rect.x + money_dest_rect.w;
    for (int i = 0, x = 0; i < money_string.length(); i++) {
      x = std::stoi(money_string.substr(i, 1));
      SDL_RenderCopy(renderer, nums_texture, &(nums_source_rect[x]), &num_dest_rect);
      num_dest_rect.x += nums_source_rect[x].w;
    }
    SDL_RenderCopy(renderer, circle, nullptr, &circle_dest_rect);
    SDL_RenderCopyEx(renderer, spinner, &spinner_rect, &spinner_dest_rect, (double) sdl_angle, &rotate_point, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) quit_pressed = true;
  }

  // Spin to stop
  delay_time = 0;
  while (quit_pressed == false && delay_time != 100) {
    if (delay_time < 100) {
      delay_time++;
      SDL_Delay(delay_time);
      if (delay_time % 5 == 0 && add > 1) add--;
    sdl_angle += add;
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, money_texture, nullptr, &money_dest_rect);
    num_dest_rect.y = money_dest_rect.y;
    money_string = std::to_string(money);
    num_dest_rect.x = money_dest_rect.x + money_dest_rect.w;
    for (int i = 0, x = 0; i < money_string.length(); i++) {
      x = std::stoi(money_string.substr(i, 1));
      SDL_RenderCopy(renderer, nums_texture, &(nums_source_rect[x]), &num_dest_rect);
      num_dest_rect.x += nums_source_rect[x].w;
    }
    SDL_RenderCopy(renderer, circle, nullptr, &circle_dest_rect);
    SDL_RenderCopyEx(renderer, spinner, &spinner_rect, &spinner_dest_rect, (double) sdl_angle, &rotate_point, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) quit_pressed = true;
  }

  // Wait for X before exiting
  do {
    SDL_Delay(20);
    SDL_RenderPresent(renderer);
    SDL_PollEvent(&e);
  } while (e.type != SDL_QUIT && quit_pressed == false);

  // Deconstruction
  SDL_DestroyTexture(circle); // Destroy circle texture
  SDL_DestroyTexture(spinner);
  SDL_DestroyTexture(money_texture);
  SDL_DestroyTexture(nums_texture);
  SDL_DestroyRenderer(renderer); // Kill renderer
  SDL_DestroyWindow(window); // Kill Window

  // Quit modules (reverse order)
  TTF_Quit(); // TrueType Fonts (render text)
  IMG_Quit(); // IMG (load from PNG files)
  SDL_Quit(); // Base SDL
  return 0;
}
