#include "screen.hpp"
#include <iostream>

Screen::Screen() {
  window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Screen::screen_width,
                            Screen::screen_height, SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                              SDL_TEXTUREACCESS_STATIC, Screen::screen_width,
                              Screen::screen_height);
  graphics.fill(0);
}

Screen::~Screen() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Screen::update() {
  // SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  std::uint32_t *pixels = nullptr;
  int pitch;
  SDL_LockTexture(texture, nullptr, reinterpret_cast<void **>(&pixels), &pitch);
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    pixels[i] = (graphics[i] == 0) ? 0x000000FF : 0xFFFFFFFF;
  }
  SDL_UnlockTexture(texture);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
  // SDL_UpdateTexture(texture, NULL, &graphics, Screen::screen_width);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
void Screen::clear() {
  // std::fill(graphics, graphics + upscale, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
byte Screen::toggle(std::array<byte, 4096> const &memory, byte n, uint16_t i,
                    byte regx, byte regy) {
  byte pixel;
  byte flipped;
  for (int j = 0; j < n; j++) {
    pixel = memory[i + j];
    for (int k = 0; k < 8; k++) {
      if (0 != (pixel & (0x80 >> k))) {
        if (1 == graphics[regx + k + ((regy + j) * 64)]) {
          flipped = 1;
        };
        // Toggle the pixel with XOR
        graphics[regx + k + (regy + j)] ^= 1;
      };
    }
  }
  return flipped;
}
