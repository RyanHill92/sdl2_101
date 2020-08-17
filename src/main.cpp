#include <iostream>
#include <memory>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "box_loop.hpp"
#include "color_box.hpp"
#include "image_texture.hpp"

using std::cout;
using std::endl;

struct Deleter {
  void operator()(SDL_Window* w) {
    cout << "Destroying window" << endl;
    SDL_DestroyWindow(w);
  }
  void operator()(SDL_Renderer* r) {
    cout << "Destroying renderer" << endl;
    SDL_DestroyRenderer(r);
  }
  void operator()(SDL_Texture* t) {
    cout << "Destroying texture" << endl;
    SDL_DestroyTexture(t);
  }
};

std::unique_ptr<SDL_Window, Deleter> window;
std::shared_ptr<SDL_Renderer> renderer;

const int screen_width = 1280;
const int screen_height = 720;

bool Init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
    return false;
  }

  window = std::unique_ptr<SDL_Window, Deleter>(SDL_CreateWindow("Textures",
                                                    SDL_WINDOWPOS_UNDEFINED,
                                                    SDL_WINDOWPOS_UNDEFINED,
                                                    screen_width,
                                                    screen_height,
                                                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
      Deleter());

  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
    return false;
  }

  renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), Deleter());

  if (!renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get renderer: %s", SDL_GetError());
    return false;
  }

  int image_flags = IMG_INIT_PNG;
  if (IMG_Init(image_flags) != image_flags) {
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL Image: %s", SDL_GetError());
    return false;
  }

  return true;
}

int main(int argc, char* args[]) {
  int code = 0;

  if (!Init()) {
    code = 1;
  } else {
    ImageTexture pretty_scene = ImageTexture(renderer, "./../media/pretty_scene.png");
    ImageTexture stick_guy =
        ImageTexture(renderer, "./../media/foo.png", ImageTextureOptions{true, 0, 255, 255});
    ImageTexture dots =
        ImageTexture(renderer, "./../media/dots.png", ImageTextureOptions{true, 0, 255, 255});

    std::vector<SDL_Rect> dot_clips;
    for (int y = 0; y < 200; y += 100) {
      for (int x = 0; x < 200; x += 100) {
        dot_clips.push_back(SDL_Rect{x, y, 100, 100});
      }
    }

    SDL_Event event;
    while (true) {
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }

        SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
        SDL_RenderClear(renderer.get());

        pretty_scene.Render();
        stick_guy.Render(screen_width / 2, screen_height * 3 / 5);

        dots.RenderClip(0, 0, dot_clips[0]);
        dots.RenderClip(screen_width - 100, 0, dot_clips[1]);
        dots.RenderClip(0, screen_height - 100, dot_clips[2]);
        dots.RenderClip(screen_width - 100, screen_height - 100, dot_clips[3]);

        SDL_RenderPresent(renderer.get());
      }
    }

    IMG_Quit();
    SDL_Quit();
    return code;
  }
}
