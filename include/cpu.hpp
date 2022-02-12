#ifndef CPU_H
#define CPU_H
#include "Controller.hpp"
#include "Ops.hpp"
#include "screen.hpp"
#include <array>
#include <cstdint>
#include <stdint.h>
#include <string>
#include <unordered_map>

#define MEMSIZE 4096
typedef std::uint8_t byte;
typedef std::uint16_t address;
class CPU {
public:
  CPU();
  CPU(Screen *screen);
  void loadROM(std::string rom_path);
  void step();
  bool shouldDraw() { return drawFlag; };
  void toggleDraw() { drawFlag = !drawFlag; };
  void toggleDraw(bool val) { drawFlag = val; };
  void reset();

  address getPC() { return pc; }

protected:
  mutable std::array<byte, 16> registers{}; // Vx
  mutable address i_reg;                    // I reg
  mutable std::array<byte, MEMSIZE> memory;
  address pc;
  std::array<address, 16> stack{};
  address stp = 0;
  address curr;

  byte delayTimer;
  byte soundTimer;

  int m_cycle;

  Controller *m_controller;
  Screen *m_screen;

  address readAddress(address a);
  OpCode getOpcode(address a);
  byte getVxFromAddress(address a) { return a >> 8 & 0x000F; };
  byte getVyFromAddress(address a) { return a >> 4 & 0x000F; };

  bool drawFlag;
};

struct Chippy final {
  // 16 Registers
  mutable std::array<byte, 16> reg{};
  // Index Register
  mutable address i;
  // Timers
  mutable byte delayTimer, soundTimer;
  // Memory
  mutable std::array<byte, MEMSIZE> memory;
  // Program Counter
  address pc;
  // Stack
  std::array<address, 16> stack{};
  // Stack Pointer
  address stp{0};
  // mutable std::stack<address> stp;
  bool drawFlag = false;

  // Current OPCODE
  address opcode;
  // Keyboard
  std::array<byte, 16> keybind{};
  const std::array<uint8_t, 16 * 5> fonts{
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  std::unordered_map<SDL_Keycode, byte> keyboardmap{
      {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
      {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
      {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
      {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}};

  Chippy();
  ~Chippy();
  [[nodiscard]] bool loadROM(const std::string &rom);
  bool prepare();
  void begin();
  void cycle();
  void exit();

private:
  Screen *screen;
};

#endif // !CPU_H