#pragma once

#include <SDL.h>
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
  bool drawPixel(byte xCoord, byte yCoord, byte spriteRow, byte spriteData);
  byte toggle(std::array<byte, 4096> const &mem, byte size, uint16_t i, byte x,
              byte y);
  void updateTitle(const std::string title) {
    SDL_SetWindowTitle(window, title.c_str());
  };

private:
  static constexpr byte upscale = 10;
  static constexpr unsigned int screen_width = 64;
  static constexpr unsigned int screen_height = 32;

  // Graphics Pixels
  std::array<byte, 64 * 32> graphics{};
  //
  // SDL RELATED
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
};
