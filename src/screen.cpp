#include "screen.hpp"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include <algorithm>

Screen::Screen() {
  window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, screen_width * upscale,
                            screen_height * upscale, SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(window, -1, 0);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

Screen::~Screen() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Screen::update() {
  std::uint32_t pixels[screen_height * screen_width];
  for (int i = 0; i < 2048; ++i) {
    pixels[i] = (0x00FFFFFF * graphics[i]) | 0xFF000000;
  }
  SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(Uint32));

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
void Screen::clear() { std::fill(std::begin(graphics), std::end(graphics), 0); }
bool Screen::drawPixel(byte xCoord, byte yCoord, byte spriteRow,
                       byte spriteData) {
  bool flipped = false;
  for (int xCol = 0; xCol < 8; xCol++) {
    if ((spriteData & (0x80 >> xCol)) != 0) {
      if (graphics[xCoord + xCol + ((yCoord + spriteRow) * 64)] == 1) {
        flipped = true;
      }
      graphics[xCoord + xCol + ((yCoord + spriteRow) * 64)] ^= 1;
    }
  }
  return flipped;
}
