#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SDL_keycode.h"
#include "screen.hpp"
#include <unordered_map>
class Controller
{
  public:
    Controller();
    bool isPressed(byte v_in);
    void handleInput(int input);

  private:
    std::array<bool, 16>                  keyBinds{};
    std::unordered_map<SDL_Keycode, byte> map{
        {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
        {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
        {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}};
};
#endif // !CONTROLLER_H
