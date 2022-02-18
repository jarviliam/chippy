#include "Emulator.hpp"
#include "cpu.hpp"
#include <iostream>
int main(int argc, char *argv[]) {
  std::string rom;
  if (argc == 1) {
    rom = "/Users/liam.jarvis/Documents/chippy/roms/IBM Logo.ch8";
  } else {
    rom = "/Users/liam.jarvis/Documents/chippy/roms/test_opcode.ch8";
  }
  // std::string temp =
  Emulator emu = Emulator();
  emu.run(rom);
  return 0;
}
