#pragma once

#include <SDL2/SDL.h>

#include <string>

enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL,
};

const int DEFAULT_SCREEN_WIDTH = 1280;
const int DEFAULT_SCREEN_HEIGHT = 720;

class ArrowGame {
 private:
  SDL_Window* window;
  int screen_width;
  int screen_height;
  SDL_Surface* screen_surface;
  SDL_Surface* image_surface;
  SDL_Surface* key_press_surfaces[KEY_PRESS_SURFACE_TOTAL];
  std::string error_msg;
  bool errored;
  bool sdl_initialized;

  void SetError(std::string msg);
  bool Initialize();
  SDL_Surface* LoadSurface(std::string path);
  bool LoadMedia();

 public:
  ArrowGame();
  ArrowGame(int screen_width, int screen_height);
  int Play();
  ~ArrowGame();
};