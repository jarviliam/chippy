#include "Emulator.hpp"
#include "cpu.hpp"
#include <iostream>
int main(int argc, char *argv[]) {
  std::string temp = "/Users/liam.jarvis/Documents/chippy/roms/IBM Logo.ch8";
  // std::string temp =
  // "/Users/liam.jarvis/Documents/chippy/roms/test_opcode.ch8";
  Emulator emu = Emulator();
  emu.run(temp);
  return 0;
}
