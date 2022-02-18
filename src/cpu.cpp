#include "cpu.hpp"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdlib.h>

address CPU::readAddress(address a) { return memory[a] << 8 | memory[a + 1]; }

OpCode CPU::getOpcode(address a) {
  switch (a & 0xF000) {
  case 0x0000:
    switch (a & NibbleMask) {
    case 0x0000:
      return CLS;
    case 0x000E:
      return RET;
    }
  case 0x1000:
    return JP;
  case 0x2000:
    return CALL;
  case 0x3000:
    return SE_BY;
  case 0x4000:
    return SNE_BY;
  case 0x5000:
    return SE_VY;
  case 0x6000:
    return LD_BY;
  case 0x7000:
    return ADD;
  case 0x8000:
    switch (a & NibbleMask) {
    case 0x0000:
      return LD_VX;
    case 0x0001:
      return OR;
    case 0x0002:
      return AND;
    case 0x0003:
      return XOR;
    case 0x0004:
      return ADD_VX;
    case 0x0005:
      return SUB_VX;
    case 0x0006:
      return SHR_VX;
    case 0x0007:
      return SUBN;
    case 0x000E:
      return SHL;
    }
  case 0x9000:
    return SNE_VX;
  case 0xA000:
    return LD_I;
  case 0xB000:
    return JP_V;
  case 0xC000:
    return RND;
  case 0xD000:
    return DRW;
  case 0xE000:
    switch (a & ByteMask) {
    case 0x009E:
      return SKP;
    case 0x00A1:
      return SKNP;
    }
  case 0xF000:
    switch (a & ByteMask) {
    case 0x0007:
      return LD_VX_DT;
    case 0x000A:
      return LD_VX_K;
    case 0x0015:
      return LD_DT;
    case 0x0018:
      return LD_ST;
    case 0x001E:
      return ADD_I;
    case 0x0029:
      return LD_F;
    case 0x0033:
      return LD_B;
    case 0x0055:
      return LD_I_VX;
    case 0x0065:
      return LD_VX_I;
    }
  default:
    throw;
  }
};

void CPU::loadROM(std::string rom_path) {
  std::ifstream rm(rom_path, std::ios::binary);
  if (!rm.is_open() || rm.bad()) {
    // Err
    throw;
  }
  std::copy(std::istreambuf_iterator<char>(rm),
            std::istreambuf_iterator<char>(), memory.begin() + 0x200);
}

void CPU::reset() {
  std::fill(std::begin(stack), std::end(stack), 0);
  std::fill(std::begin(registers), std::end(registers), 0);
  std::fill(std::begin(memory), std::end(memory), 0);
  pc = 0x200;
};

void CPU::step() {
  m_cycle++;
  address instruction = readAddress(pc);
  pc += 2;
  switch (getOpcode(instruction)) {

  case SYS: {
    break;
  }

  case CLS: {
    m_screen->clear();
    // drawFlag = true;
    break;
  }

  case RET: {
    assert(stp > 0);
    pc = stack[stp - 1] + 2;
    stp--;
    break;
  }

  case JP: {
    address target = instruction & AddrMask;
    pc = target;
    break;
  }

  case CALL: {
    address target = instruction & AddrMask;
    stack[stp] = pc - 2;
    stp++;
    pc = target;
    break;
  }

  case SE_BY: {
    byte target = instruction & ByteMask;
    byte vx = getVxFromAddress(instruction);
    if (registers[vx] == target) {
      pc += 2;
    }
    break;
  }

  case SNE_BY: {
    byte target = instruction & ByteMask;
    byte vx = getVxFromAddress(instruction);
    if (registers[vx] != target) {
      pc += 2;
    }
    break;
  }

  case SE_VY: {
    byte vy = getVyFromAddress(instruction);
    byte vx = getVxFromAddress(instruction);
    if (registers[vx] == registers[vy]) {
      pc += 2;
    }
    break;
  }

  case LD_BY: {
    byte vx = getVxFromAddress(instruction);
    byte target = instruction & ByteMask;
    registers[vx] = target;
    break;
  }

  case ADD: {
    byte vx = getVxFromAddress(instruction);
    byte target = instruction & ByteMask;
    registers[vx] += target;
    break;
  }

  case LD_VX: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[vx] = registers[vy];
    break;
  }
  case OR: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[vx] |= registers[vy];
    break;
  }

  case AND: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[vx] &= registers[vy];
    break;
  }

  case XOR: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[vx] ^= registers[vy];
    break;
  }
  case ADD_VX: {
    // TODO() test
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    // byte res = registers[vx] + registers[vy];
    registers[15] = (registers[vy] > (0xFF - registers[vx])) ? 1 : 0;
    registers[vx] += registers[vy];
    break;
  }
  case SUB_VX: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[15] = registers[vx] > registers[vy] ? 1 : 0;
    registers[vx] -= registers[vy];
    break;
  }
  case SHR_VX: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[0xF] = registers[vx] & 0x1;
    registers[vx] = registers[vx] >> 1;
    break;
  }
  case SUBN: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    registers[15] = registers[vy] > registers[vx] ? 1 : 0;
    registers[vx] = registers[vy] - registers[vx];
    break;
  }
  case SHL: {
    byte vx = getVxFromAddress(instruction);
    registers[15] = (registers[vx] >> 7) & 0x1;
    registers[vx] <<= 1;
    break;
  }
  case SNE_VX: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    if (registers[vx] != registers[vy]) {
      pc += 2;
    }
    break;
  }
  case LD_I: {
    i_reg = instruction & AddrMask;
    break;
  }
  case JP_V: {
    pc = registers[0] + (instruction & AddrMask);
    break;
  }
  case RND: {
    byte vx = getVxFromAddress(instruction);
    registers[vx] = (rand() % 0xFF) & ByteMask;
    break;
  }
  case DRW: {
    byte vx = getVxFromAddress(instruction);
    byte vy = getVyFromAddress(instruction);
    byte nRows = instruction & NibbleMask;
    byte yCoord = registers[vy] % 32;
    byte xCoord = registers[vx] % 64;
    // Set VF to 0
    registers[15] = 0;
    for (int row = 0; row < nRows; row++) {
      byte pixel = memory[i_reg + row];
      if (m_screen->drawPixel(xCoord, yCoord, row, pixel)) {
        registers[15] = 1;
      }
    }
    drawFlag = true;
    break;
  }
  case SKP: {
    byte vx = getVxFromAddress(instruction);
    if (m_controller->isPressed(registers[vx])) {
      pc += 2;
    }
    break;
  }
  case SKNP: {
    byte vx = getVxFromAddress(instruction);
    if (!m_controller->isPressed(registers[vx])) {
      pc += 2;
    }
    break;
  }
  case LD_VX_DT: {
    byte vx = getVxFromAddress(instruction);
    registers[vx] = delayTimer;
    break;
  }

  case LD_VX_K: {
    byte vx = getVxFromAddress(instruction);
    // pc-=2;
    auto pressed = m_controller->getPressed();
    if (pressed.has_value()) {
      auto val = pressed.value();
      registers[vx] = val;
    } else {
      pc -= 2;
    }
    break;
  }

  case LD_DT: {
    byte vx = getVxFromAddress(instruction);
    delayTimer = registers[vx];
    break;
  }

  case LD_ST: {
    byte vx = getVxFromAddress(instruction);
    soundTimer = registers[vx];
    break;
  }

  case ADD_I: {
    byte vx = getVxFromAddress(instruction);
    i_reg += registers[vx];
    break;
  }

  case LD_F: {
    byte vx = getVxFromAddress(instruction);
    i_reg = registers[vx] * 5;
    break;
  }

  case LD_B: {
    byte vx = getVxFromAddress(instruction);
    memory[i_reg] = registers[vx] / 100;
    memory[i_reg + 1] = (registers[vx] / 10) % 10;
    memory[i_reg + 2] = (registers[vx] % 100) % 10;
    break;
  }

  case LD_I_VX: {
    byte vx = getVxFromAddress(instruction);
    for (int i = 0; i <= vx; ++i) {
      memory[i_reg + i] = registers[i];
    }
    break;
  }

  case LD_VX_I: {
    byte vx = getVxFromAddress(instruction);
    for (int i = 0; i <= vx; ++i) {
      registers[i] = memory[i_reg + i];
    }
    break;
  }

  default:
    std::cout << "Invalid OP" << instruction << '\n';
    throw;
  }
}

CPU::CPU() {
  for (int i = 0; i < 80; ++i)
    memory[i] = chip8_fontset[i];
}
CPU::CPU(Screen *screen) { m_screen = screen; }
