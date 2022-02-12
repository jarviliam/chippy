#include "Emulator.hpp"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include <fstream>
#include <iterator>

Emulator::Emulator() {
  m_screen = new Screen();
  m_cpu = new CPU(m_screen);
}
Emulator::~Emulator() {
  delete m_cpu;
  delete m_screen;
}

void Emulator::run(std::string rom_path) {
  m_cpu->reset();
  m_cpu->loadROM(rom_path);
  std::string title = rom_path.substr(rom_path.find_last_of('/') + 1,
                                      rom_path.find_first_of(".ch8"));

  bool loop = true;
  SDL_Event e;
  while (loop) {
    while (SDL_PollEvent(&e) > 0) {
      switch (e.type) {
      case SDL_KEYDOWN: {
        SDL_Keycode input = e.key.keysym.sym;
        m_controller.handleInput(input);
        break;
      }
      case SDL_KEYUP: {
        const SDL_Keycode input = e.key.keysym.sym;
        m_controller.handleInput(input);
        break;
      }
      case SDL_QUIT:
        loop = false;
        break;
      default:
        break;
      };
    };
    m_cpu->step();

    if (m_cpu->shouldDraw()) {
      m_screen->update();
      m_cpu->toggleDraw();
    }
  };
}
