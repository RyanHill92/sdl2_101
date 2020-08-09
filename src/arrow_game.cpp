#include "arrow_game.hpp"

#include <SDL2/SDL_image.h>

#include <iostream>
#include <sstream>
#include <vector>

void ArrowGame::SetError(std::string msg) {
  error_msg = msg;
  errored = true;
}

bool ArrowGame::Initialize() {
  window = nullptr;
  screen_surface = nullptr;
  image_surface = nullptr;

  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SetError("Error initializing SDL");
    success = false;
  }

  sdl_initialized = true;

  window = SDL_CreateWindow("Arrow Game",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      screen_width,
      screen_height,
      SDL_WINDOW_SHOWN);

  if (!window) {
    SetError("Window could not be created");
    success = false;
  }

  screen_surface = SDL_GetWindowSurface(window);
  if (!screen_surface) {
    SetError("Could not get window surface");
    success = false;
  }

  return success;
}

SDL_Surface* ArrowGame::LoadSurface(std::string path) {
  SDL_Surface* optimizedImage = nullptr;

  SDL_Surface* originalImage = IMG_Load(path.c_str());
  if (originalImage) {
    optimizedImage = SDL_ConvertSurface(originalImage, screen_surface->format, 0);
    SDL_FreeSurface(originalImage);
  }

  return optimizedImage;
}

bool ArrowGame::LoadMedia() {
  bool success = true;
  std::vector<std::string> file_names{"big_owl.png", "up.bmp", "down.bmp", "left.bmp", "right.bmp"};

  for (int i = KEY_PRESS_SURFACE_DEFAULT; i < KEY_PRESS_SURFACE_TOTAL; i++) {
    std::stringstream ss;
    ss << "./../media/" << file_names[i];

    key_press_surfaces[i] = LoadSurface(ss.str());
    if (!key_press_surfaces[i]) {
      SetError("Failed to load image from path " + ss.str());
      success = false;
    }
  }

  // Set default image.
  image_surface = key_press_surfaces[0];

  return success;
}

// Not sure if there's a way to share a constructor overload.
ArrowGame::ArrowGame(int screen_width_, int screen_height_)
    : screen_width(screen_width_), screen_height(screen_height_) {
  if (!Initialize() || !LoadMedia()) {
    std::cout << error_msg << ": " << SDL_GetError() << std::endl;
  }
}

ArrowGame::ArrowGame() : screen_width(DEFAULT_SCREEN_WIDTH), screen_height(DEFAULT_SCREEN_HEIGHT) {
  if (!Initialize() || !LoadMedia()) {
    std::cout << error_msg << ": " << SDL_GetError() << std::endl;
  }
}

int ArrowGame::Play() {
  if (errored) {
    return 1;
  }

  bool quit = false;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_UP:
              image_surface = key_press_surfaces[KEY_PRESS_SURFACE_UP];
              break;
            case SDLK_DOWN:
              image_surface = key_press_surfaces[KEY_PRESS_SURFACE_DOWN];
              break;
            case SDLK_LEFT:
              image_surface = key_press_surfaces[KEY_PRESS_SURFACE_LEFT];
              break;
            case SDLK_RIGHT:
              image_surface = key_press_surfaces[KEY_PRESS_SURFACE_RIGHT];
              break;
            default:
              SDL_FillRect(
                  screen_surface, nullptr, SDL_MapRGB(screen_surface->format, 0x00, 0x00, 0x00));
              image_surface = key_press_surfaces[KEY_PRESS_SURFACE_DEFAULT];
              break;
          }
      }
    }

    // The tutorial has us passing a destination SDL_Rectangle, but it achieves the exact
    // same as blitting the entire source rectangle to the entire destination rectangle,
    // i.e. using nullptr for both rectangle params.

    // SDL_Rect dst_rect;
    // dst_rect.x = 0;
    // dst_rect.y = 0;
    // dst_rect.w = SCREEN_WIDTH;
    // dst_rect.h = SCREEN_HEIGHT;

    SDL_BlitScaled(image_surface, nullptr, screen_surface, nullptr);
    SDL_UpdateWindowSurface(window);
  }

  return 0;
}

ArrowGame::~ArrowGame() {
  std::cout << "Destructing ArrowGame" << std::endl;
  if (image_surface) {
    SDL_FreeSurface(image_surface);
    image_surface = nullptr;
    std::cout << "Image surface freed" << std::endl;
  }

  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
    std::cout << "Window destroyed" << std::endl;
  }

  if (sdl_initialized) {
    SDL_Quit();
    std::cout << "SDL quitted" << std::endl;
  }
  std::cout << "ArrowGame destructed" << std::endl;
}