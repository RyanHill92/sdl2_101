#include "box_loop.hpp"

#include <iostream>

void BoxLoop::ToggleColorIfClicked(ColorBox& box, int mouse_x, int mouse_y) {
  if (box.Clicked(mouse_x, mouse_y)) {
    box.NextColor();
  }
}

BoxLoop::BoxLoop(std::shared_ptr<SDL_Renderer> renderer, int screen_width, int screen_height)
    : _renderer(renderer), _screen_width(screen_width), _screen_height(screen_height) {
  _left_viewport = {0, 0, _screen_width / 2, _screen_height};
  _right_viewport = {_screen_width / 2, 0, _screen_width / 2, _screen_height};

  _b1 = ColorBox{_left_viewport, 0, 0, _screen_width / 30, _screen_height / 20};
  _b2 = ColorBox{_right_viewport, 0, 0, _screen_width / 30, _screen_height / 20};

  std::cout << "Pointer shared " << _renderer.use_count() << " times in BoxLoop constructor"
            << std::endl;
}

BoxLoop::~BoxLoop() {
  std::cout << "Pointer shared " << _renderer.use_count() << " times in BoxLoop destructor"
            << std::endl;
}

void BoxLoop::Start() {
  std::cout << "Pointer shared " << _renderer.use_count() << " times in BoxLoop Start method"
            << std::endl;

  ColorBox& box_one = _b1;
  ColorBox& box_two = _b2;

  std::vector<ColorBox> boxes = {box_one, box_two};

  while (true) {
    if (SDL_PollEvent(&_event)) {
      if (_event.type == SDL_QUIT) {
        break;
      }
      if (_event.type == SDL_KEYDOWN) {
        switch (_event.key.keysym.sym) {
          case SDLK_w:
            box_one.Up();
            break;
          case SDLK_s:
            box_one.Down();
            break;
          case SDLK_a:
            box_one.Left();
            break;
          case SDLK_d:
            box_one.Right();
            break;
          case SDLK_UP:
            box_two.Up();
            break;
          case SDLK_DOWN:
            box_two.Down();
            break;
          case SDLK_LEFT:
            box_two.Left();
            break;
          case SDLK_RIGHT:
            box_two.Right();
            break;
          default:
            break;
        }
      }

      if (_event.type == SDL_MOUSEBUTTONDOWN) {
        ToggleColorIfClicked(box_one, _event.button.x, _event.button.y);
        ToggleColorIfClicked(box_two, _event.button.x, _event.button.y);
      }
    }

    SDL_SetRenderDrawColor(_renderer.get(), 255, 255, 255, 255);
    SDL_RenderClear(_renderer.get());

    // Left Side
    SDL_RenderSetViewport(_renderer.get(), &_left_viewport);
    SDL_SetRenderDrawColor(_renderer.get(), 255, 0, 255, 255);
    SDL_Rect outline;

    for (int i = 12; i > 2; i /= 2) {
      outline = {_left_viewport.w / i,
          _left_viewport.h / i,
          _left_viewport.w * (i - 2) / i,
          _left_viewport.h * (i - 2) / i};
      SDL_RenderDrawRect(_renderer.get(), &outline);
    }

    SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);

    int x = _left_viewport.w / 3;
    int y = _left_viewport.h / 3;
    for (int j = x + (x / 20); j < _left_viewport.w * 2 / 3; j += (x / 20)) {
      for (int k = y + (y / 20); k < _left_viewport.h * 2 / 3; k += (y / 20)) {
        SDL_RenderDrawPoint(_renderer.get(), j, k);
      }
    }

    SDL_SetRenderDrawColor(_renderer.get(), box_one.r(), box_one.g(), box_one.b(), box_one.a());
    SDL_Rect rect_one = box_one.rect();
    SDL_RenderFillRect(_renderer.get(), &rect_one);

    // Right Side
    SDL_RenderSetViewport(_renderer.get(), &_right_viewport);
    SDL_SetRenderDrawColor(_renderer.get(), 192, 192, 192, 255);
    SDL_Rect whole_screen = {0, 0, _right_viewport.w, _right_viewport.h};
    SDL_RenderFillRect(_renderer.get(), &whole_screen);

    SDL_SetRenderDrawColor(_renderer.get(), box_two.r(), box_two.g(), box_two.b(), box_two.a());
    SDL_Rect rect_two = box_two.rect();
    SDL_RenderFillRect(_renderer.get(), &rect_two);

    SDL_RenderPresent(_renderer.get());
  }

  return;
}