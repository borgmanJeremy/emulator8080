#include "cpu_8080.hpp"

void Cpu_8080::addValToReg(uint8_t val, uint8_t &reg)
{
  uint16_t result = static_cast<uint16_t>(static_cast<uint16_t>(reg) +
                                          static_cast<uint16_t>(val));
  setZeroFlag(result);
  setCarryFlag(result);
  setSignFlag(result);
  setParityFlag(result);

  uint8_t ac_test = (reg & 0x0F) + (val & 0x0F);
  setAuxFlag(ac_test);

  reg = static_cast<uint8_t>(result);
}

void Cpu_8080::doubleAddToHL(uint8_t upper_byte, uint8_t lower_byte)
{
  int32_t val_1 = (static_cast<uint32_t>(reg_.h) << 8) + reg_.l;
  int32_t val_2 = (static_cast<uint32_t>(upper_byte) << 8) + lower_byte;

  auto result = val_1 + val_2;
  reg_.l = static_cast<uint8_t>(result & 0xFF);
  reg_.h = static_cast<uint8_t>((result >> 8) & 0xFF);

  flags_.cy = (result > 0xFFFF) ? 1 : 0;
}

void Cpu_8080::addRegToRegA(uint8_t to_add) { addValToReg(to_add, reg_.a); }

void Cpu_8080::addRegToRegAWithCarry(uint8_t to_add)
{
  addRegToRegA(static_cast<uint8_t>(to_add + flags_.cy));
}

void Cpu_8080::addMathAddOperations()
{
  instruction_set_.emplace_back(Instruction{0x80, 0, "ADD B", [this]() {
                                              addRegToRegA(reg_.b);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x81, 0, "ADD C", [this]() {
                                              addRegToRegA(reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x82, 0, "ADD D", [this]() {
                                              addRegToRegA(reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x83, 0, "ADD E", [this]() {
                                              addRegToRegA(reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x84, 0, "ADD H", [this]() {
                                              addRegToRegA(reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x85, 0, "ADD L", [this]() {
                                              addRegToRegA(reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x86, 0, "ADD M", [this]() {
                                              auto addr = getAddressFromHL();
                                              addRegToRegA(memory_[addr]);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x87, 0, "ADD A", [this]() {
                                              addRegToRegA(reg_.a);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xC6, 1, "ADI D8", [this]() {
                                              addRegToRegA(
                                                memory_[reg_.pc + 1]);
                                              reg_.pc += 2U;
                                            }});

  instruction_set_.emplace_back(Instruction{0x88, 0, "ADC B", [this]() {
                                              addRegToRegAWithCarry(reg_.b);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x89, 0, "ADC C", [this]() {
                                              addRegToRegAWithCarry(reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x8A, 0, "ADC D", [this]() {
                                              addRegToRegAWithCarry(reg_.d);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x8B, 0, "ADC E", [this]() {
                                              addRegToRegAWithCarry(reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x8C, 0, "ADC H", [this]() {
                                              addRegToRegAWithCarry(reg_.h);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x8D, 0, "ADC L", [this]() {
                                              addRegToRegAWithCarry(reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0xCE, 1, "ACI D8", [this]() {
                                              addRegToRegAWithCarry(
                                                memory_[reg_.pc + 1]);
                                              reg_.pc += 2;
                                            }});

  instruction_set_.emplace_back(Instruction{0x8E, 0, "ADC M", [this]() {
                                              auto addr = getAddressFromHL();
                                              addRegToRegAWithCarry(
                                                memory_[addr]);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x8F, 0, "ADC A", [this]() {
                                              addRegToRegAWithCarry(reg_.a);
                                              reg_.pc++;
                                            }});

  // Double add
  instruction_set_.emplace_back(Instruction{9, 0, "DAD B", [this]() {
                                              doubleAddToHL(reg_.b, reg_.c);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x19, 0, "DAD D", [this]() {
                                              doubleAddToHL(reg_.d, reg_.e);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(Instruction{0x29, 0, "DAD H", [this]() {
                                              doubleAddToHL(reg_.h, reg_.l);
                                              reg_.pc++;
                                            }});

  instruction_set_.emplace_back(
    Instruction{0x39, 0, "DAD SP", [this]() {
                  doubleAddToHL(static_cast<uint8_t>((reg_.sp >> 8) & 0xFF),
                                static_cast<uint8_t>(reg_.sp & 0xFF));
                  reg_.pc++;
                }});

  instruction_set_.emplace_back(Instruction{
    0x27, 0, "DAA", [this]() {
      uint8_t lower_four = (0x0F & reg_.a);
      if ((lower_four > 9) || flags_.ac == 1)
      {
        reg_.a += 6;
        flags_.ac = 1;
      }
      else
      {
        flags_.ac = 0;
      }

      uint8_t upper_four = (0xF0 & reg_.a) >> 4;
      if ((flags_.cy == 1) || (upper_four > 9))
      {
        auto carryover = static_cast<unsigned int>(reg_.a) + (0x06 << 4);
        reg_.a = static_cast<uint8_t>(carryover & 0xFF);
        flags_.cy = 1;
      }

      reg_.pc++;
    }});
}