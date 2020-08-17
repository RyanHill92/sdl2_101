#include "image_texture.hpp"

#include <string>

#include "SDL2/SDL_image.h"

void ImageTexture::LoadImage(std::string image_path) {
  SDL_Surface* image = IMG_Load(image_path.c_str());
  if (!image) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Failed to load image %s: %s",
        image_path.c_str(),
        SDL_GetError());
    return;
  }

  _image_width = image->w;
  _image_height = image->h;

  if (_options.key_by_color) {
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, _options.r, _options.g, _options.b));
  }

  _texture = std::unique_ptr<SDL_Texture, ImageTextureDeleter>(
      SDL_CreateTextureFromSurface(_renderer.get(), image), ImageTextureDeleter());

  SDL_FreeSurface(image);

  if (!_texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Failed to create texture from image %s: %s",
        image_path.c_str(),
        SDL_GetError());
  }

  _canRender = true;
}

ImageTexture::ImageTexture(std::shared_ptr<SDL_Renderer> renderer, std::string image_path)
    : _renderer(renderer) {
  LoadImage(image_path);
}

ImageTexture::ImageTexture(
    std::shared_ptr<SDL_Renderer> renderer, std::string image_path, ImageTextureOptions options)
    : _renderer(renderer), _options(options) {
  LoadImage(image_path);
}

bool ImageTexture::Validate() const {
  if (!_canRender) {
    std::cout << "Unable to render because initialization failed. Check STDERR for details."
              << std::endl;
    return false;
  }

  return true;
}

// Render the entire texture to the entire rendering target.
void ImageTexture::Render() const {
  if (Validate()) {
    SDL_RenderCopy(_renderer.get(), _texture.get(), nullptr, nullptr);
  }
}

// Render the texture to a point on the rendering target using
// source image width and height.
void ImageTexture::Render(int x, int y) const {
  if (Validate()) {
    SDL_Rect dst = {x, y, _image_width, _image_height};
    SDL_RenderCopy(_renderer.get(), _texture.get(), nullptr, &dst);
  }
}

// Render a specific clip of the source image to a point on the
// rendering target.
void ImageTexture::RenderClip(int x, int y, SDL_Rect clip) const {
  if (Validate()) {
    SDL_Rect dst = {x, y, clip.w, clip.h};
    SDL_RenderCopy(_renderer.get(), _texture.get(), &clip, &dst);
  }
}