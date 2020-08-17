#pragma once

#include <SDL2/SDL.h>

#include <vector>

class ColorBox {
 private:
  std::vector<unsigned char> _rgba;
  SDL_Rect _viewport;
  int _x, _y;
  int _w, _h;

 public:
  ColorBox();
  ColorBox(SDL_Rect viewport, int w, int h);
  ColorBox(SDL_Rect viewport, int x, int y, int w, int h);
  ~ColorBox();

  void Up();
  void Down();
  void Left();
  void Right();
  bool Clicked(int mouse_x, int mouse_y) const;
  void NextColor();

  SDL_Rect rect() const { return SDL_Rect{_x, _y, _w, _h}; }

  unsigned char r() const { return _rgba[0]; }
  unsigned char g() const { return _rgba[1]; }
  unsigned char b() const { return _rgba[2]; }
  unsigned char a() const { return _rgba[3]; }

  void set_r(unsigned char r) { _rgba[0] = r; }
  void set_g(unsigned char g) { _rgba[1] = g; }
  void set_b(unsigned char b) { _rgba[2] = b; }
  void set_a(unsigned char a) { _rgba[3] = a; }
};