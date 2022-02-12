#include "cpu.hpp"
#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>

class ChippyAccessor : public CPU::CPU {
public:
  ChippyAccessor() {
    this->pc = 0x200;
    stp = 0;
  }
  const std::array<byte, 4096> &get_memory() const { return memory; };
  bool DF() const { return drawFlag; };
  address PC() const { return pc; };
  byte REGAT(int i) const { return registers[i]; };
  address IREG() const { return i_reg; };
  address STAT() const { return stack[stp]; };
  address STP() const { return stp; };
  void setReg(int i, byte in) { registers[i] = in; }
  void loadBuffer(std::vector<byte> in) {
    for (size_t i = 0; i < in.size(); i++) {
      memory[i + 512] = in[i];
    }
  };
};
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(opcode, CLS) {
  ChippyAccessor chip;
  std::vector<byte> src{0x00, 0xE0};

  chip.loadBuffer(src);
  // chip.step();

  // ASSERT_EQ(chip.DF(), true);
};

TEST(opcode, JMP) {
  ChippyAccessor chip;
  std::vector<byte> src{0x1A, 0xBC};

  chip.loadBuffer(src);
  // byte pre = chip.PC();
  chip.step();

  ASSERT_EQ(chip.PC(), 0xABC);
}

TEST(opcode, RET) {
  ChippyAccessor chip;
  std::vector<byte> src{0x00, 0x0E};
  chip.loadBuffer(src);
  // chip.step();

  // ASSERT_EQ(chip.STP(), )
  // ASSERT_EQ(chip.PC(), )
  // stp-- causes overflow at 0;
}

TEST(opcode, CALL) {
  ChippyAccessor chip;
  std::vector<byte> src{0x28, 0x23};
  chip.loadBuffer(src);
  ASSERT_EQ(0, chip.STP());
  auto prePC = chip.PC();
  chip.step();

  ASSERT_EQ(chip.STP(), 1);
  ASSERT_EQ(chip.STAT(), prePC);
  ASSERT_EQ(chip.PC(), 0x823);
}

TEST(opcode, SEBY) {
  ChippyAccessor chip;
  std::vector<byte> src{0x33, 0x23};
  chip.loadBuffer(src);
  auto prePC = chip.PC();
  chip.setReg(3, 0x23);
  chip.step();
  ASSERT_EQ(chip.REGAT(3), 0x23);
  ASSERT_EQ(chip.PC() - prePC, 4);
}
TEST(opcode, SNEBY) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SEVY) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDBY) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, ADD) {
  ChippyAccessor chip;
  std::vector<byte> src{0x73, 0x55};
  chip.loadBuffer(src);
  auto preval = chip.REGAT(3);
  byte prePC = chip.PC();
  chip.step();

  ASSERT_NE(preval, chip.REGAT(3));
  ASSERT_EQ(chip.REGAT(3), 0x55);
  ASSERT_NE(chip.PC(), prePC);
}
TEST(opcode, LDVX) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, OR) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, AND) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, XOR) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, ADDVX) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SUBVX) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SHRVX) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SUBN) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SHL) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SNEVX) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
// Load index
TEST(opcode, LDI) {
  ChippyAccessor chip;
  std::vector<byte> src{0xA0, 0x12};
  chip.loadBuffer(src);
  byte priIreg = chip.IREG();
  byte prePC = chip.PC();
  chip.step();

  ASSERT_NE(priIreg, chip.IREG());
  ASSERT_EQ(chip.IREG(), 0x012);
  ASSERT_NE(prePC, chip.PC());
}
TEST(opcode, JPV) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, RND) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, DRW) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SKP) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, SKNP) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDVXDT) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDVXK) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDDT) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDST) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, ADDI) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDF) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDB) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDIVX) {
  ChippyAccessor chip;
  std::vector<byte> src{0x21, 0x23};
  chip.loadBuffer(src);
}
TEST(opcode, LDVXI) {
  ChippyAccessor chip;
  std::vector<byte> src{0xF1, 0x65};
  chip.loadBuffer(src);

  chip.step();
}
