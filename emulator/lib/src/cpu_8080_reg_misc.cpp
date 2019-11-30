#include "cpu_8080.hpp"

void Cpu_8080::addRegMisc()
{
  instruction_set_.emplace_back(Instruction{0xf3, 0, "DI", [this]() {
                                              int_enable_ = 0;
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xfb, 0, "EI", [this]() {
                                              int_enable_ = 1;
                                              reg_.pc++;
                                            }});

  // Rotates
  instruction_set_.emplace_back(
    Instruction{7, 0, "RLC", [this]() {
                  // set carry to upper bit value
                  flags_.cy = ((reg_.a & 0x80) == 0x80) ? 1 : 0;

                  reg_.a <<= 1;
                  // If upper bit was one, rotate to
                  // 0 bit
                  if (flags_.cy == 1)
                  {
                    reg_.a |= 0x01;
                  }
                  reg_.pc++;
                }});

  instruction_set_.emplace_back(
    Instruction{0x0F, 0, "RRC", [this]() {
                  flags_.cy = ((reg_.a & 0x01) == 0x01) ? 1 : 0;
                  reg_.a >>= 1;
                  // If upper bit was one, rotate to
                  // 0 bit
                  if (flags_.cy == 1)
                  {
                    reg_.a |= 0x80;
                  }

                  reg_.pc++;
                }});

  instruction_set_.emplace_back(
    Instruction{0x17, 0, "RAL", [this]() {
                  bool upper_bit_set = ((reg_.a & 0x80) == 0x80) ? true : false;
                  bool flag_was_set = (flags_.cy == 1) ? true : false;
                  flags_.cy = (upper_bit_set) ? 1 : 0;
                  reg_.a <<= 1;
                  if (flag_was_set)
                  {
                    reg_.a |= 0x01;
                  }

                  reg_.pc++;
                }});

  instruction_set_.emplace_back(
    Instruction{0x1F, 0, "RAR", [this]() {
                  bool lower_bit_set = ((reg_.a & 0x01) == 0x01) ? true : false;
                  bool flag_was_set = (flags_.cy == 1) ? true : false;
                  flags_.cy = (lower_bit_set) ? 1 : 0;
                  reg_.a >>= 1;
                  if (flag_was_set)
                  {
                    reg_.a |= 0x80;
                  }
                  reg_.pc++;
                }});

  // INX
  instruction_set_.emplace_back(Instruction{
    3, 0, "INX B", [this]() {
      unsigned int addition = (static_cast<unsigned int>(reg_.b) << 8) + reg_.c;
      addition++;
      reg_.b = static_cast<uint8_t>(addition >> 8);
      reg_.c = static_cast<uint8_t>(addition & 0x00FF);
      reg_.pc++;
    }});

  instruction_set_.emplace_back(Instruction{
    0x13, 0, "INX D", [this]() {
      unsigned int addition = (static_cast<unsigned int>(reg_.d) << 8) + reg_.e;
      addition++;
      reg_.d = static_cast<uint8_t>(addition >> 8);
      reg_.e = static_cast<uint8_t>(addition & 0x00FF);
      reg_.pc++;
    }});

  instruction_set_.emplace_back(Instruction{
    0x23, 0, "INX H", [this]() {
      unsigned int addition = (static_cast<unsigned int>(reg_.h) << 8) + reg_.l;
      addition++;
      reg_.h = static_cast<uint8_t>(addition >> 8);
      reg_.l = static_cast<uint8_t>(addition & 0x00FF);
      reg_.pc++;
    }});

  instruction_set_.emplace_back(Instruction{0x33, 0, "INX SP", [this]() {
                                              reg_.sp++;
                                              reg_.pc++;
                                            }});

  // INR
  instruction_set_.emplace_back(Instruction{4, 0, "INR B", [this]() {
                                              addValToReg(1, reg_.b);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x0C, 0, "INR C", [this]() {
                                              addValToReg(1, reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x14, 0, "INR D", [this]() {
                                              addValToReg(1, reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x1C, 0, "INR E", [this]() {
                                              addValToReg(1, reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x24, 0, "INR H", [this]() {
                                              addValToReg(1, reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x2C, 0, "INR L", [this]() {
                                              addValToReg(1, reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x34, 0, "INR M", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = memory_[addr] + 1;
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x3C, 0, "INR A", [this]() {
                                              addValToReg(1, reg_.a);
                                              reg_.pc++;
                                            }});

  // DRC
  instruction_set_.emplace_back(Instruction{5, 0, "DCR B", [this]() {
                                              subValToReg(1, reg_.b);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x0D, 0, "DCR C", [this]() {
                                              subValToReg(1, reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x15, 0, "DCR D", [this]() {
                                              subValToReg(1, reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x1D, 0, "DCR E", [this]() {
                                              subValToReg(1, reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x25, 0, "DCR H", [this]() {
                                              subValToReg(1, reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x2D, 0, "DCR L", [this]() {
                                              subValToReg(1, reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x35, 0, "DCR M", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = memory_[addr] - 1;
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x3D, 0, "DCR A", [this]() {
                                              subValToReg(1, reg_.a);
                                              reg_.pc++;
                                            }});

  // Misc
  instruction_set_.emplace_back(Instruction{0x37, 0, "STC", [this]() {
                                              flags_.cy = 1;
                                              reg_.pc++;
                                            }});
}