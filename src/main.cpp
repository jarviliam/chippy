#include "cpu.hpp"
#include <iostream>
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Arg issue" << '\n';
    return 1;
  }
  Chippy cpu = Chippy();
  if (!cpu.loadROM(argv[1])) {
    std::cout << "Invalid ROM" << '\n';
    return 1;
  };

  if (!cpu.prepare()) {
    printf("SDL Video Error");
    return 1;
  }
  if(!cpu.loadROM(argv[1])){
    printf("ROM LOAD ERR");
    return 1;
  };
  cpu.begin();
  return 0;
}
