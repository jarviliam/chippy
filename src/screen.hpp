#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <stdint.h>
#include <string>

typedef uint8_t byte;

class Screen {
public:
  Screen();
  ~Screen();
  void update();
  void clear();
  byte toggle(byte *mem, byte size, uint16_t i, byte x, byte y);
  void updateTitle(const std::string title) {
    SDL_SetWindowTitle(window, title.c_str());
  };

private:
  static constexpr byte upscale = 20;
  static constexpr unsigned int screen_width = 64 * upscale;
  static constexpr unsigned int screen_height = 32 * upscale;

  // Graphics Pixels
  std::array<byte, (upscale * 64) * (upscale * 32)> graphics{};
  //
  // SDL RELATED
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
};
