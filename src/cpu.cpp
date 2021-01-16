#include "cpu.hpp"

Chippy::Chippy(){

};

bool Chippy::start() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Invalid SDL Video");
    return false;
  }
  window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Chippy::screen_width,
                            Chippy::screen_height, SDL_WINDOW_SHOWN);
  return true;
}
void Chippy::exit(){};

void Chippy::step(){

};
bool Chippy::loadROM(const std::string &rom) { return true; };
