#include "cpu_8080.hpp"

void Cpu_8080::andRegToRegA(uint8_t to_and)
{
  reg_.a = reg_.a & to_and;
  setZeroFlag(reg_.a);
  setSignFlag(reg_.a);
  setParityFlag(reg_.a);
  flags_.cy = 0;
}

void Cpu_8080::orRegToRegA(uint8_t to_or)
{
  reg_.a = reg_.a | to_or;
  setZeroFlag(reg_.a);
  setSignFlag(reg_.a);
  setParityFlag(reg_.a);
  flags_.cy = 0;
}

void Cpu_8080::xorRegToRegA(uint8_t to_xor)
{
  reg_.a = reg_.a ^ to_xor;
  setZeroFlag(reg_.a);
  setSignFlag(reg_.a);
  setParityFlag(reg_.a);
  flags_.cy = 0;
}

void Cpu_8080::compRegToRegA(uint8_t to_comp)
{
  auto result =
    static_cast<unsigned int>(reg_.a) - static_cast<unsigned int>(to_comp);

  setZeroFlag(result);
  setSignFlag(result);
  setParityFlag(result);
  setCarryFlag(result);
}

void Cpu_8080::addLogicalOperations()
{
  instruction_set_.emplace_back(Instruction{0x2F, 0, "CMA", [this]() {
                                              reg_.a = ~reg_.a;
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x3F, 0, "CMC", [this]() {
                                              flags_.cy =
                                                (flags_.cy == 1) ? 0 : 1;
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA0, 0, "ANA B", [this]() {
                                              andRegToRegA(reg_.b);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA1, 0, "ANA C", [this]() {
                                              andRegToRegA(reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA2, 0, "ANA D", [this]() {
                                              andRegToRegA(reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA3, 0, "ANA E", [this]() {
                                              andRegToRegA(reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA4, 0, "ANA H", [this]() {
                                              andRegToRegA(reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA5, 0, "ANA L", [this]() {
                                              andRegToRegA(reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA6, 0, "ANA M", [this]() {
                                              auto addr = getAddressFromHL();
                                              andRegToRegA(memory_[addr]);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xE6, 1, "ANI D8", [this]() {
                                              andRegToRegA(
                                                memory_[reg_.pc + 1]);
                                              reg_.pc += 2;
                                            }});

  instruction_set_.emplace_back(Instruction{0xA7, 0, "ANA A", [this]() {
                                              andRegToRegA(reg_.a);
                                              reg_.pc++;
                                            }});

  ////////////
  instruction_set_.emplace_back(Instruction{0xA8, 0, "XRA B", [this]() {
                                              xorRegToRegA(reg_.b);
                                              reg_.pc++;
                                            }});
  instruction_set_.emplace_back(Instruction{0xA9, 0, "XRA C", [this]() {
                                              xorRegToRegA(reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xAA, 0, "XRA D", [this]() {
                                              xorRegToRegA(reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xAB, 0, "XRA E", [this]() {
                                              xorRegToRegA(reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xAC, 0, "XRA H", [this]() {
                                              xorRegToRegA(reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xAD, 0, "XRA L", [this]() {
                                              xorRegToRegA(reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xEE, 1, "XRI D8", [this]() {
                                              xorRegToRegA(
                                                memory_[reg_.pc + 1]);
                                              reg_.pc += 2;
                                            }});

  instruction_set_.emplace_back(Instruction{0xAE, 0, "XRA M", [this]() {
                                              auto addr = getAddressFromHL();
                                              xorRegToRegA(memory_[addr]);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xAF, 0, "XRA A", [this]() {
                                              xorRegToRegA(reg_.a);
                                              reg_.pc++;
                                            }});

  ////////////
  instruction_set_.emplace_back(Instruction{0xB0, 0, "ORA B", [this]() {
                                              orRegToRegA(reg_.b);
                                              reg_.pc++;
                                            }});
  instruction_set_.emplace_back(Instruction{0xB1, 0, "ORA C", [this]() {
                                              orRegToRegA(reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB2, 0, "ORA D", [this]() {
                                              orRegToRegA(reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB3, 0, "ORA E", [this]() {
                                              orRegToRegA(reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB4, 0, "ORA H", [this]() {
                                              orRegToRegA(reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB5, 0, "ORA L", [this]() {
                                              orRegToRegA(reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xf6, 1, "ORI D8", [this]() {
                                              orRegToRegA(memory_[reg_.pc + 1]);
                                              reg_.pc += 2;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB6, 0, "ORA M", [this]() {
                                              auto addr = getAddressFromHL();
                                              orRegToRegA(memory_[addr]);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB7, 0, "ORA A", [this]() {
                                              orRegToRegA(reg_.a);
                                              reg_.pc++;
                                            }});
  ////////////
  instruction_set_.emplace_back(Instruction{0xB8, 0, "CMP B", [this]() {
                                              compRegToRegA(reg_.b);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xB9, 0, "CMP C", [this]() {
                                              compRegToRegA(reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xBA, 0, "CMP D", [this]() {
                                              compRegToRegA(reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xBB, 0, "CMP E", [this]() {
                                              compRegToRegA(reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xBC, 0, "CMP H", [this]() {
                                              compRegToRegA(reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xBD, 0, "CMP L", [this]() {
                                              compRegToRegA(reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xBE, 0, "CMP M", [this]() {
                                              auto addr = getAddressFromHL();
                                              compRegToRegA(memory_[addr]);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xBF, 0, "CMP A", [this]() {
                                              compRegToRegA(reg_.a);
                                              reg_.pc++;
                                            }});
}
