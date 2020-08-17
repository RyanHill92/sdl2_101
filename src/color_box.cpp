#include "color_box.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>

ColorBox::ColorBox() {}

ColorBox::ColorBox(SDL_Rect viewport, int w, int h) : _viewport(viewport), _w(w), _h(h) {
  _x = 0;
  _y = 0;
  _rgba = {0, 255, 128, 255};
}

ColorBox::ColorBox(SDL_Rect viewport, int x, int y, int w, int h)
    : _viewport(viewport), _x(x), _y(y), _w(w), _h(h) {
  _rgba = {0, 255, 128, 255};
}

ColorBox::~ColorBox() {}

void ColorBox::Up() {
  if (_y > 0) {
    int new_y = _y - _h;
    if (new_y < 0) {
      new_y = 0;
    }
    _y = new_y;
  }
}

void ColorBox::Down() {
  if ((_y + _h) < _viewport.h) {
    int new_y = _y + _h;
    if ((new_y + _h) > _viewport.h) {
      new_y = _viewport.h - _h;
    }
    _y = new_y;
  }
}

void ColorBox::Left() {
  if (_x > 0) {
    int new_x = _x - _w;
    if (new_x < 0) {
      new_x = 0;
    }
    _x = new_x;
  }
}

void ColorBox::Right() {
  if ((_x + _w) < _viewport.w) {
    int new_x = _x + _w;
    if ((new_x + _w) > _viewport.w) {
      new_x = _viewport.w - _w;
    }
    _x = new_x;
  }
}

bool ColorBox::Clicked(int mouse_x, int mouse_y) const {
  int true_x = mouse_x - _viewport.x;
  bool x_in_range = _x <= true_x && true_x <= (_x + _w);
  if (!x_in_range) {
    return false;
  }

  int true_y = mouse_y - _viewport.y;
  bool y_in_range = _y <= true_y && true_y <= (_y + _h);
  if (!y_in_range) {
    return false;
  }

  return true;
}

void ColorBox::NextColor() {
  std::transform(std::begin(_rgba), std::end(_rgba) - 1, std::begin(_rgba), [](unsigned char c) {
    switch (c) {
      case 0:
        return 64;
      case 64:
        return 128;
      case 128:
        return 192;
      case 192:
        return 255;
      case 255:
      default:
        return 0;
    }
  });

  std::iter_swap(std::begin(_rgba), std::begin(_rgba) + 1);
  std::iter_swap(std::rbegin(_rgba) + 1, std::rbegin(_rgba) + 2);

  std::cout << "RGBA: ";
  for (auto c : _rgba) {
    std::cout << int(c) << ' ';
  }
  std::cout << std::endl;
}