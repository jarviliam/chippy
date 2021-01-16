#include <SDL2/SDL.h>
#include <array>
#include <stack>
#include <stdint.h>
#include <string>

#define MEMSIZE 4096
typedef uint8_t byte;
typedef uint16_t address;

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
  address stp;
  // mutable std::stack<address> stp;

  // Current OPCODE
  address opcode;

  // Graphics Pixels
  std::array<byte, 64 * 32> graphics{};
  // Keyboard
  std::array<byte, 16> keybind{};
  const std::array<uint8_t, 16*5>fonts{
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
  static constexpr unsigned int upscale = 20;
  static constexpr unsigned int screen_width = 64 * upscale;
  static constexpr unsigned int screen_height = 32 * upscale;

  Chippy();
  ~Chippy();
  [[nodiscard]] bool loadROM(const std::string &rom);
  bool start();
  void step();
  void exit();

private:
  // SDL RELATED
  SDL_Window *window;
};
