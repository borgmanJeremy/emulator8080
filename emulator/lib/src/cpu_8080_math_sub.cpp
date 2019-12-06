#include "cpu_8080.hpp"

void Cpu_8080::subRegToRegA(uint8_t to_sub) { subValToReg(to_sub, reg_.a); }

void Cpu_8080::subValToReg(uint8_t val, uint8_t &reg)
{
  uint16_t result = static_cast<uint16_t>(static_cast<uint16_t>(reg) -
                                          static_cast<uint16_t>(val));
  setZeroFlag(result);
  setCarryFlag(result);
  setSignFlag(result);
  setParityFlag(result);

  uint8_t ac_test = (reg & 0x0F) - (val & 0x0F);
  setAuxFlag(ac_test);

  reg = static_cast<uint8_t>(result);
}

void Cpu_8080::subRegToRegAWithBorrow(uint8_t to_sub)
{
  subRegToRegA(static_cast<uint8_t>(to_sub + flags_.cy));
}

void Cpu_8080::addMathSubOperations()
{
  instruction_set_.emplace_back(Instruction{0x90, 0, "SUB B", [this]() {
                                              subRegToRegA(reg_.b);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});
  instruction_set_.emplace_back(Instruction{0x91, 0, "SUB C", [this]() {
                                              subRegToRegA(reg_.c);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x92, 0, "SUB D", [this]() {
                                              subRegToRegA(reg_.d);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x93, 0, "SUB E", [this]() {
                                              subRegToRegA(reg_.e);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x94, 0, "SUB H", [this]() {
                                              subRegToRegA(reg_.h);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x95, 0, "SUB L", [this]() {
                                              subRegToRegA(reg_.l);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x96, 0, "SUB M", [this]() {
                                              auto addr = getAddressFromHL();
                                              subRegToRegA(memory_[addr]);
                                              reg_.pc++;
                                              cycle_count_ += 7;
                                            }});

  instruction_set_.emplace_back(Instruction{0xd6, 1, "SUI D8", [this]() {
                                              subRegToRegA(
                                                memory_[reg_.pc + 1]);
                                              reg_.pc += 2;
                                              cycle_count_ += 7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x97, 0, "SUB A", [this]() {
                                              subRegToRegA(reg_.a);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  ////////////
  instruction_set_.emplace_back(Instruction{0x98, 0, "SBB B", [this]() {
                                              subRegToRegAWithBorrow(reg_.b);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x99, 0, "SBB C", [this]() {
                                              subRegToRegAWithBorrow(reg_.c);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x9A, 0, "SBB D", [this]() {
                                              subRegToRegAWithBorrow(reg_.d);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x9B, 0, "SBB E", [this]() {
                                              subRegToRegAWithBorrow(reg_.e);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x9C, 0, "SBB H", [this]() {
                                              subRegToRegAWithBorrow(reg_.h);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0x9D, 0, "SBB L", [this]() {
                                              subRegToRegAWithBorrow(reg_.l);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  instruction_set_.emplace_back(Instruction{0xde, 1, "SBI D8", [this]() {
                                              subRegToRegAWithBorrow(
                                                memory_[reg_.pc + 1]);
                                              reg_.pc += 2;
                                              cycle_count_ += 7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x9E, 0, "SBB M", [this]() {
                                              auto addr = getAddressFromHL();
                                              subRegToRegAWithBorrow(
                                                memory_[addr]);
                                              reg_.pc++;
                                              cycle_count_ += 7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x9F, 0, "SBB A", [this]() {
                                              subRegToRegAWithBorrow(reg_.a);
                                              reg_.pc++;
                                              cycle_count_ += 4;
                                            }});

  // Decrement double
  instruction_set_.emplace_back(
    Instruction{0x0B, 0, "DCX B", [this]() {
                  unsigned int subtraction =
                    (static_cast<unsigned int>(reg_.b) << 8) - reg_.c;
                  subtraction--;
                  reg_.b = static_cast<uint8_t>(subtraction >> 8);
                  reg_.c = static_cast<uint8_t>(subtraction & 0x00FF);
                  reg_.pc++;
                  cycle_count_ += 5;
                }});

  instruction_set_.emplace_back(
    Instruction{0x1B, 0, "DCX D", [this]() {
                  unsigned int subtraction =
                    (static_cast<unsigned int>(reg_.d) << 8) - reg_.e;
                  subtraction--;
                  reg_.d = static_cast<uint8_t>(subtraction >> 8);
                  reg_.e = static_cast<uint8_t>(subtraction & 0x00FF);
                  reg_.pc++;
                  cycle_count_ += 5;
                }});

  instruction_set_.emplace_back(
    Instruction{0x2B, 0, "DCX H", [this]() {
                  unsigned int subtraction =
                    (static_cast<unsigned int>(reg_.h) << 8) + reg_.l;
                  subtraction--;
                  reg_.h = static_cast<uint8_t>(subtraction >> 8);
                  reg_.l = static_cast<uint8_t>(subtraction & 0x00FF);
                  reg_.pc++;
                  cycle_count_ += 5;
                }});

  instruction_set_.emplace_back(Instruction{0x3B, 0, "DCX SP", [this]() {
                                              reg_.sp--;
                                              reg_.pc++;

                                              cycle_count_ += 5;
                                            }});

  instruction_set_.emplace_back(Instruction{
    0xfe, 1, "CPI D8", [this]() {
      uint16_t result =
        static_cast<uint16_t>(static_cast<uint16_t>(reg_.a) -
                              static_cast<uint16_t>(memory_[reg_.pc + 1]));
      setZeroFlag(result);
      setCarryFlag(result);
      setSignFlag(result);
      setParityFlag(result);

      uint8_t ac_test = (reg_.a & 0x0F) - (memory_[reg_.pc + 1] & 0x0F);
      setAuxFlag(ac_test);

      reg_.pc += 2;
      cycle_count_ += 7;
    }});
}