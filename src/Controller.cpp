#include "Controller.hpp"
#include "SDL_keycode.h"
#include "screen.hpp"

bool Controller::isPressed(byte v_in) { return keyBinds[v_in]; };

Controller::Controller() {}

void Controller::handleInput(int input)
{
    if (map.find((SDL_Keycode)input) == map.end()) {
        // TODO Error Out
        return;
    }
    byte target = map[input];
    keyBinds[target] ^= keyBinds[target];
};
