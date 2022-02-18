#include "Controller.hpp"
#include "SDL_keycode.h"
#include "screen.hpp"
#include <optional>

bool Controller::isPressed(byte v_in) { return keyBinds[v_in]; };

Controller::Controller() {}

void Controller::handleInput(int input) {
  if (map.find((SDL_Keycode)input) == map.end()) {
    // TODO Error Out
    return;
  }
  byte target = map[input];
  keyBinds[target] ^= keyBinds[target];
};

std::optional<byte> Controller::getPressed() {
  for (int i = 0; i < 16; i++) {
    if (keyBinds[i]) {
      return map[i];
    }
  }
  return std::nullopt;
}
