#include "cpu.hpp"
#include <fstream>
#include <stdlib.h>

Chippy::Chippy(){

};
Chippy::~Chippy(){

};

bool Chippy::prepare() {
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
  // SDL_Surface *surface = SDL_GetWindowSurface(window);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Failed to make Renderer\n");
    return false;
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                              SDL_TEXTUREACCESS_STATIC, Chippy::screen_width,
                              Chippy::screen_height);
  if (!texture) {
    printf("Failed to make Texture\n");
    return false;
  }

  // Start Up Memory;
  return true;
}

void Chippy::begin() {
  bool loop = true;
  SDL_Event e;
  while (loop) {
    while (SDL_PollEvent(&e) > 0) {
      switch (e.type) {
      case SDL_QUIT:
        loop = false;
        break;
      case SDL_KEYDOWN: {
        const SDL_Keycode key = e.key.keysym.sym;
        if (key == SDLK_ESCAPE)
          loop = false;
        if (keyboardmap.find(key) != keyboardmap.end()) {
          keyboardmap[key] = 1;
        }
        break;
      };
      case SDL_KEYUP: {
        const SDL_Keycode key = e.key.keysym.sym;
        if (keyboardmap.find(key) != keyboardmap.end()) {
          keyboardmap[key] = 0;
        }
        break;
      }
      default:
        break;
      };
    };
    cycle();

    if (drawFlag) {
    printf("Draw Flag\n");
      // SDL_UpdateTexture
      SDL_UpdateTexture(texture, NULL, &graphics, Chippy::screen_width);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
      drawFlag = false;
    }
  };
}

void Chippy::exit() {
  SDL_DestroyWindow(window);
  SDL_Quit();
};

void Chippy::cycle() {
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
      printf("CLR\n");
      SDL_SetRenderDrawColor(renderer,0,0,0,0);
      SDL_RenderClear(renderer);
      pc += 2;
      drawFlag = true;
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
     //printf("JMP %X\n", nnn);
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
    // Set's VX to nn
  case 0x6000:
    printf("SETVX\n");
    reg[x] = nn;
    pc += 2;
    break;
    // Add's nn to VX
  case 0x7000:
    printf("ADDVX\n");
    reg[x] += nn;
    pc += 2;
    break;
    // Setting Value of  VX with VY
  case 0x8000:
    switch (n) {
    case 0x0000:
      // Sets VX to the value of VY.
      printf("SETVXVY\n");
      reg[x] = reg[y];
      pc += 2;
      break;
    case 0x0001:
      // Sets VX to VX or VY. (Bitwise OR operation)
      printf("SETVXORVY\n");
      reg[x] |= reg[y];
      pc += 2;
      break;
    case 0x0002:
      // Sets VX to VX and VY. (Bitwise AND operation)
      printf("SETVXANDVY\n");
      reg[x] &= reg[y];
      pc += 2;
      break;
    case 0x0003:
      // Sets VX to VX xor VY.
      printf("SETVXXORVY\n");
      reg[x] ^= reg[y];
      pc += 2;
      break;
    case 0x0004:
      // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there
      // isn't.
      printf("ADDVYTOVX\n");
      reg[15] = (reg[y] > (0xFF - reg[x])) ? 1 : 0;
      reg[x] += reg[y];
      pc += 2;
      break;
    case 0x0005:
      // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1
      // when there isn't
      printf("VXSUBVY\n");
      reg[15] = (reg[y] > reg[x]) ? 0 : 1;
      reg[x] -= reg[y];
      pc += 2;
      break;
    case 0x0006:
      // Stores the least significant bit of VX in VF and then shifts VX to the
      // right by 1
      printf("VXVFSTORE\n");
      reg[15] = reg[x] & 0x000F;
      reg[x] >>= 1;
      pc += 2;
      break;
    case 0x0007:
      // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1
      // when there isn't.
      printf("SETVXMINVY\n");
      reg[15] = (reg[x] > reg[y]) ? 0 : 1;
      reg[x] = reg[y] - reg[x];
      pc += 2;
      break;
    case 0x000E:
      // Stores the most significant bit of VX in VF and then shifts VX to the
      // left by 1
      printf("STORESHIFT \n");
      // TODO: Check if 0xF000 is signficant
      reg[15] = reg[x] & 0xF000;
      reg[x] <<= 1;
      pc += 2;
      break;
    }
    break;
  case 0x9000:
    printf("SKIP \n");
    // Skips the next instruction if VX doesn't equal VY. (Usually the next
    // instruction is a jump to skip a code block)
    if (reg[x] != reg[y]) {
      pc += 4;
    } else {
      pc += 2;
    }
    break;
  case 0xA000:
    printf("SETI \n");
    // Sets I to the address NNN.
    i = nnn;
    pc += 2;
    break;
  case 0xB000:
    printf("SETNNN \n");
    // Jumps to the address NNN plus V0.
    pc = nnn + reg[0];
    break;
  case 0xC000:
    printf("RAND \n");
    // Sets VX to the result of a bitwise and operation on a random number
    // (Typically: 0 to 255) and NN.
    reg[x] = (rand() % 0xFF) & nn;
    pc += 2;
    break;
  case 0xD000: {
    /**
     *  	Draws a sprite at coordinate (VX, VY) that has a width of 8
     * pixels and a height of N+1 pixels. Each row of 8 pixels is read as
     * bit-coded starting from memory location I; I value doesn’t change after t
     * e ex ecution of this instruction. As described above, VF is set to 1 if
     * any sc reen pixels are flipped from set to unset when the sprite is
     * drawn, and t 0 if that doesn’t happen ^ VF = 1 | 1 -> 0 else 0
     */
    printf("DRAW REG%X, REG%X, REG%X\n", x, y, n);
    uint8_t regx = reg[x];
    uint8_t regy = reg[y];
    uint8_t pixel;
    reg[15] = 0;
    for (int j = 0; j < n; j++) {
      pixel = memory[i + j];
      for (int k = 0; k < 8; k++) {
        if (0 != (pixel & (0x80 >> k))) {
          if (1 == graphics[regx + k + ((regy + j) * 64)]) {
            reg[15] = 1;
          };
          graphics[regx + k + ((regy + j) * 64)] ^= 1;
        };
      }
    }
    // Have to Draw
    drawFlag = true;
    pc += 2;
    break;
  };

  case 0xE000:
    switch (nn) {
    case 0x009E: {
      // Skips the next instruction if the key stored in VX is pressed. (Usually
      // the next instruction is a jump to skip a code block)
      printf("SKIP REG%X\n", x);
      if (keybind[reg[x]] != 0) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    }
    case 0x00A1: {
      // Skips the next instruction if the key stored in VX isn't pressed.
      // (Usually the next instruction is a jump to skip a code block)
      printf("SKIP NO REG%X\n", x);
      if (keybind[reg[x]] == 0) {
        pc += 4;
      } else {
        pc += 2;
      }
      break;
    }
    };
    break;

  case 0xF000:
    switch (nn) {
    case 0x0007: {
      // Sets VX to the value of the delay timer.
      printf("SET REG%X DELAY \n", x);
      reg[x] = delayTimer;
      pc += 2;
      break;
    }
    case 0x000A: {
      // A key press is awaited, and then stored in VX. (Blocking Operation. All
      // instruction halted until next key event)
      printf("GETKEY REG%X \n", x);
      bool pressed = false;
      for (int k = 0; k < 16; ++k) {
        if (keybind[k] != 0) {
          pressed = true;
          reg[x] = k;
        }
      };
      // Blocking
      if (!pressed) {
        return;
      }
      pc += 2;
      break;
    }
    case 0x0015: {
      // Sets the delay timer to VX.
      printf("SET REG%X DELAY%X \n", x, delayTimer);
      delayTimer = reg[x];
      pc += 2;
      break;
    }
    case 0x0018: {
      // ets the sound timer to VX.
      printf("SET SOUND TO REG%X \n", x);
      soundTimer = reg[x];
      pc += 2;
      break;
    }
    case 0x001E: {
      // dds VX to I. VF is not affected.[c]
      printf("SET REG%X to %X \n", x, i);
      // VF Exeption case
      reg[15] = (i + reg[x] > 0xFFF) ? 1 : 0;
      i += reg[x];
      pc += 2;
      break;
    }
    case 0x0029: {
      // Sets I to the location of the sprite for the character in VX.
      // Characters 0-F (in hexadecimal) are represented by a 4x5 font.
      printf("SPRITE_ADDR REG%X \n", x);
      i = reg[x] & 0x5;
      pc += 2;
      break;
    }
    case 0x0033: {
      /**
       *Stores the binary-coded decimal representation of VX, with the most
       *significant of three digits at the address in I, the middle digit at I
       *plus 1, and the least significant digit at I plus 2. (In other words,
       *take the decimal representation of VX, place the hundreds digit in
       *memory at location in I, the tens digit at location I+1, and the ones
       *digit at location I+2.)
       */
      printf("SETBCD REG%X \n", x);
      memory[i] = reg[x] / 10;
      memory[i + 1] = (reg[x] / 10) % 10;
      memory[i + 2] = (reg[x] % 100) % 10;
      pc += 2;
      break;
    }
    case 0x0055: {
      // Stores V0 to VX (including VX) in memory starting at address I. The
      // offset from I is increased by 1 for each value written, but I itself is
      // left unmodified.[d]
      printf("REGDMP REG%X \n", x);
      for (int k = 0; k <= x; ++k)
        memory[k + i] = reg[k];
      i += x + 1;
      pc += 2;
      break;
    }
    case 0x0065: {
      // Fills V0 to VX (including VX) with values from memory starting at
      // address I. The offset from I is increased by 1 for each value written,
      // but I itself is left unmodified.[d]
      printf("REGFILL REG%X \n", x);
      for (int k = 0; k <= x; k++)
        reg[k] = memory[i + k];
      i += x + 1;
      pc += 2;
      break;
    }
    default:
      printf("Unknown [0xF000]: 0x%X\n", opcode);
    }
    break;
  default:
    printf("Unknown OPCODE 0x%X\n", opcode);
  };
};

bool Chippy::loadROM(const std::string &rom) {
  std::ifstream rm(rom, std::ios::binary);

  if (!rm.is_open() || rm.bad()) {
    return false;
  };
  SDL_SetWindowTitle(window, "Test");
  std::string title =
      rom.substr(rom.find_last_of('/') + 1, rom.find_first_of(".ch8"));

  std::copy(std::istreambuf_iterator<char>(rm),
            std::istreambuf_iterator<char>(), memory.begin() + 0x0200);

  rm.close();
  return true;
};
