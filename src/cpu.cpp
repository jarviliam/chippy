#include "cpu.hpp"

Chippy::Chippy(){

};

bool Chippy::start() {
  pc = 0x200; // First 512 bytes are reserved

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Invalid SDL Video");
    return false;
  }
  window = SDL_CreateWindow("Chippy", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Chippy::screen_width,
                            Chippy::screen_height, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Failed to make Window");
    return false;
  };

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  if (!surface) {
    printf("Failed to make Surface");
  }
  return true;
}
void Chippy::exit(){};

void Chippy::step() {
  SDL_Event e;
  bool loop = true;

  /**
   * Fetch Opcode.
   * Since Instructions are 16bit
   * and memory holds 8bits. We Fetch two,
   * by performing bitwise left shift with OR
   * 16 = x << 8 | y
   */
  opcode = memory[pc] << 8 | memory[pc + 1];

  /**
   * Decode Opcode
   * Get the first half-byte with a mask
   * Switch to each instruction
   */

  // OpCode Table
  uint8_t n = opcode & 0x000F;    // Lowest 4bits/nibble
  uint8_t nn = opcode & 0x00FF;   // Lowest bits/byte
  uint16_t nnn = opcode & 0x0FFF; // Lowest 12bits

  /**
   * Register Identifiers
   * 0XY0
   * Obtain X by bitshifting 8 with a mask
   * Obtain Y by bitshifting 4 with a mask
   */
  uint8_t x = opcode >> 8 & 0x000F;
  uint8_t y = opcode >> 4 & 0x000F;
  switch (opcode & 0xF000) {
  // Calls Machine Code Routine
  case 0x0000:
    switch (n) {
    // Clear Screen
    case 0x0000:
      printf("Clear Screen Inst");
      pc += 2;
      break;
      // Returns from a subroutine
    case 0x000E:
      printf("Return From SUB");
      --stp;
      pc = stack[stp];
      pc += 2;
      break;
    }
    break;
    // Jumps to address NNN
  case 0x1000:
    printf("JMP %X\n", nnn);
    pc = nnn;
    break;
    // Calls Subroutine at NNN
  case 0x2000:
    printf("CALL %X\n", nnn);
    stack[stp] = pc; // Store Return Address
    ++stp;           // Increment Pointer
    pc = nnn;        // Move the Counter to Address
    break;
    // Skips the next Instruction if VX = NN
  case 0x3000:
    printf("SKIP\n");
    if (nn == reg[x]) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
    // Skips the next instruction if VX != NN
  case 0x4000:
    printf("!SKIP\n");
    if (reg[x] != nn) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
    // Skips the next instructio if VX equals VY
  case 0x5000:
    printf("SKIPEQ\n");
    if (reg[x] == reg[y]) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0x6000:
    break;
  case 0x7000:
    break;
  case 0x8000:
    break;
  case 0x9000:
    break;
  case 0xA000:
    break;
  case 0xB000:
    break;
  case 0xC000:
    break;
  case 0xD000:
    break;
  case 0xE000:
    break;
  case 0xF000:
    break;
  };

  while (loop) {
    while (SDL_PollEvent(&e) > 0) {
      switch (e.type) {
      case SDL_QUIT:
        loop = false;
        break;
      }
      SDL_UpdateWindowSurface(window);
    };
  }
};
bool Chippy::loadROM(const std::string &rom) { return true; };
