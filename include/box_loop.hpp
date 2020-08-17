#pragma once

#include <memory>

#include "SDL2/SDL.h"
#include "color_box.hpp"

class BoxLoop {
 private:
  std::shared_ptr<SDL_Renderer> _renderer;
  int _screen_width;
  int _screen_height;
  SDL_Event _event;
  SDL_Rect _left_viewport;
  SDL_Rect _right_viewport;
  ColorBox _b1;
  ColorBox _b2;

  void ToggleColorIfClicked(ColorBox& box, int mouse_x, int mouse_y);

 public:
  // If pointer passed in, not this class's job to clean it up.
  BoxLoop(std::shared_ptr<SDL_Renderer> renderer, int screen_width, int screen_height);
  ~BoxLoop();
  void Start();
};