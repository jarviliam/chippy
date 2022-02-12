#ifndef EMULATOR_H
#define EMULATOR_H
#include "Controller.hpp"
#include "cpu.hpp"
#include <string>

class Emulator
{
  public:
    void run(std::string rom_path);
    void setScale(float scale);

    Emulator();
    ~Emulator();

  private:
    CPU        *m_cpu;
    Screen    *m_screen;
    Controller m_controller;
};

#endif // !EMULATOR_H
