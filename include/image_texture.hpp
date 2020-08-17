#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "SDL2/SDL.h"

struct ImageTextureDeleter {
  void operator()(SDL_Texture* t) {
    std::cout << "Destroying image texture" << std::endl;
    SDL_DestroyTexture(t);
  }
};

struct ImageTextureOptions {
  bool key_by_color;
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

class ImageTexture {
 private:
  int _image_width;
  int _image_height;
  std::unique_ptr<SDL_Texture, ImageTextureDeleter> _texture;
  std::shared_ptr<SDL_Renderer> _renderer;
  ImageTextureOptions _options;
  bool _canRender;

  void LoadImage(std::string image_path);

 public:
  ImageTexture(std::shared_ptr<SDL_Renderer> renderer, std::string image_path);
  ImageTexture(
      std::shared_ptr<SDL_Renderer> renderer, std::string image_path, ImageTextureOptions options);
  bool Validate() const;
  void Render() const;
  void Render(int x, int y) const;
  void RenderClip(int x, int y, SDL_Rect clip) const;
};