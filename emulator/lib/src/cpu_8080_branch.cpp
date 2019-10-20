
#include "cpu_8080.hpp"

void Cpu_8080::returnSP()
{
  unsigned int return_val = (static_cast<unsigned int>(popFromStack()) +
                             (static_cast<unsigned int>(popFromStack()) << 8));
  reg_.pc = return_val;
}

void Cpu_8080::callRetOnStack()
{
  auto return_addr = reg_.pc + 3;
  auto jump_addr = getAddressFromTwoBytes(reg_.pc + 1);

  pushToStack(static_cast<uint8_t>((return_addr & 0xFF00) >> 8));
  pushToStack(static_cast<uint8_t>((return_addr & 0xFF)));
  reg_.pc = jump_addr;
}

void Cpu_8080::resetRetOnStack(unsigned int return_address)
{
  auto return_addr = reg_.pc + 1;

  pushToStack(static_cast<uint8_t>((return_addr & 0xFF00) >> 8));
  pushToStack(static_cast<uint8_t>((return_addr & 0xFF)));
  reg_.pc = return_address;
}
void Cpu_8080::addBranchOperations()
{
  // Resets
  instruction_set_.emplace_back(
    Instruction{0xC7, 0, "RST 0", [this]() { resetRetOnStack(0x00); }});

  instruction_set_.emplace_back(
    Instruction{0xCF, 0, "RST 1", [this]() { resetRetOnStack(0x08); }});

  instruction_set_.emplace_back(
    Instruction{0xd7, 0, "RST 2", [this]() { resetRetOnStack(0x10); }});

  instruction_set_.emplace_back(
    Instruction{0xdf, 0, "RST 3", [this]() { resetRetOnStack(0x18); }});

  instruction_set_.emplace_back(
    Instruction{0xe7, 0, "RST 4", [this]() { resetRetOnStack(0x20); }});

  instruction_set_.emplace_back(
    Instruction{0xef, 0, "RST 5", [this]() { resetRetOnStack(0x28); }});

  instruction_set_.emplace_back(
    Instruction{0xf7, 0, "RST 6", [this]() { resetRetOnStack(0x30); }});

  instruction_set_.emplace_back(
    Instruction{0xff, 0, "RST 7", [this]() { resetRetOnStack(0x38); }});

  // Calls
  instruction_set_.emplace_back(
    Instruction{0xCD, 2, "CALL adr", [this]() { callRetOnStack(); }});

  instruction_set_.emplace_back(Instruction{0xC4, 2, "CNZ adr", [this]() {
                                              if (flags_.z == 0)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xCC, 2, "CZ adr", [this]() {
                                              if (flags_.z == 1)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xd4, 2, "CNC ADR", [this]() {
                                              if (flags_.cy == 0)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xdc, 2, "CC adr", [this]() {
                                              if (flags_.cy == 1)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xe4, 2, "CPO adr", [this]() {
                                              if (flags_.p == 0)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xec, 2, "CPE adr", [this]() {
                                              if (flags_.p == 1)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xf4, 2, "CP adr", [this]() {
                                              if (flags_.s == 0)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xfc, 2, "CM adr", [this]() {
                                              if (flags_.s == 1)
                                              {
                                                callRetOnStack();
                                              }
                                              else
                                              {
                                                reg_.pc += 3;
                                              }
                                            }});

  // Returns
  instruction_set_.emplace_back(
    Instruction{0xC9, 0, "RET", [this]() { returnSP(); }});

  instruction_set_.emplace_back(Instruction{0xC0, 0, "RNZ", [this]() {
                                              if (flags_.z == 0)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xC8, 0, "RZ", [this]() {
                                              if (flags_.z == 1)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xD0, 0, "RNC", [this]() {
                                              if (flags_.cy == 0)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xd8, 0, "RC", [this]() {
                                              if (flags_.cy == 1)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xe0, 0, "RPO", [this]() {
                                              if (flags_.p == 0)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xe8, 0, "RPE", [this]() {
                                              if (flags_.p == 1)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xf0, 0, "RP", [this]() {
                                              if (flags_.s == 0)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  instruction_set_.emplace_back(Instruction{0xf8, 0, "RM", [this]() {
                                              if (flags_.s == 1)
                                              {
                                                returnSP();
                                              }
                                              else
                                              {
                                                reg_.pc += 1;
                                              }
                                            }});

  // Jumps
  instruction_set_.emplace_back(Instruction{0xe9, 0, "PCHL", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.pc = addr;
                                            }});

  instruction_set_.emplace_back(
    Instruction{0xC3, 2, "JMP adr", [this]() {
                  auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                  reg_.pc = addr;
                }});

  instruction_set_.emplace_back(
    Instruction{0xC2, 2, "JNZ adr", [this]() {
                  if (flags_.z == 0)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xCA, 2, "JZ adr", [this]() {
                  if (flags_.z == 1)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xd2, 2, "JNC adr", [this]() {
                  if (flags_.cy == 0)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xda, 2, "JC adr", [this]() {
                  if (flags_.cy == 1)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xe2, 2, "JPO adr", [this]() {
                  if (flags_.p == 0)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xea, 2, "JPE adr", [this]() {
                  if (flags_.p != 0)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xf2, 2, "JP adr", [this]() {
                  if (flags_.s == 0)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});

  instruction_set_.emplace_back(
    Instruction{0xfa, 2, "JM adr", [this]() {
                  if (flags_.s != 0)
                  {
                    auto addr = getAddressFromTwoBytes(reg_.pc + 1);
                    reg_.pc = addr;
                  }
                  else
                  {
                    reg_.pc += 3;
                  }
                }});
}