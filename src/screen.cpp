#include "screen.hpp"

Screen::Screen() {
  window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Screen::screen_width,
                            Screen::screen_height, SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                              SDL_TEXTUREACCESS_STATIC, Screen::screen_width,
                              Screen::screen_height);
}

Screen::~Screen() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Screen::update() {
  SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
  SDL_RenderClear(renderer);
  // SDL_UpdateTexture
  SDL_UpdateTexture(texture, NULL, &graphics, Screen::screen_width);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
void Screen::clear() {
  // graphics.cl
}
byte Screen::toggle(byte *memory, byte n, uint16_t i, byte regx, byte regy) {
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
