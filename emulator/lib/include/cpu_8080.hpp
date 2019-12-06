#ifndef CPU_8080_HPP_
#define CPU_8080_HPP_

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "memory.hpp"
#include "smart_counter.hpp"

struct Instruction
{
  uint8_t op_code;
  uint8_t num_args;
  std::string instruction;
  std::function<void()> exp;
};

struct Registers
{
  uint8_t a = 0;
  uint8_t b = 0;
  uint8_t c = 0;
  uint8_t d = 0;
  uint8_t e = 0;
  uint8_t h = 0;
  uint8_t l = 0;
  uint16_t sp = 0;
  uint16_t pc = 0;
  bool operator==(const Registers& reg) const
  {
    return ((reg.a == a) && (reg.b == b) && (reg.c == c) && (reg.d == d) &&
            (reg.e == e) && (reg.h == h) && (reg.l == l) && (reg.sp == sp) &&
            (reg.pc == pc));
  }
};

struct Flags
{
  uint8_t z = 0;
  uint8_t s = 0;
  uint8_t p = 0;
  uint8_t cy = 0;
  uint8_t ac = 0;
};

class Cpu_8080
{
 public:
  Cpu_8080();
  ~Cpu_8080() = default;

  void passCommandArgs(std::vector<uint8_t> const& args);
  uint16_t getAddressFromHL() const;
  uint16_t getAddressFromBC() const;
  uint16_t getAddressFromDE() const;

  uint16_t getAddressFromTwoBytes(unsigned int base_address) const;

  std::vector<Instruction> instruction_set_;
  Registers reg_;
  Flags flags_;
  std::vector<uint8_t> output_port_;
  std::vector<uint8_t> input_port_;

  smart_memory::Memory<uint8_t> memory_;
  uint8_t int_enable_;

  unsigned long int cycle_count_;
  // SmartCounter<unsigned long int> cycle_count_;

 private:
  static constexpr unsigned int port_size_ = 8;

  void addRegToRegA(uint8_t to_add);
  void addRegToRegAWithCarry(uint8_t to_add);

  void addValToReg(uint8_t val, uint8_t& reg);
  void doubleAddToHL(uint8_t upper_byte, uint8_t lower_byte);

  void subRegToRegA(uint8_t to_sub);
  void subRegToRegAWithBorrow(uint8_t to_sub);
  void subValToReg(uint8_t val, uint8_t& reg);

  void andRegToRegA(uint8_t to_and);
  void orRegToRegA(uint8_t to_or);
  void xorRegToRegA(uint8_t to_xor);
  void compRegToRegA(uint8_t to_comp);

  void setZeroFlag(uint16_t result);
  void setSignFlag(uint16_t result);
  void setCarryFlag(uint16_t result);
  void setParityFlag(uint16_t result);
  void setAuxFlag(uint8_t result);

  void addMathAddOperations();
  void addMathSubOperations();
  void addMoveOperations();
  void addLogicalOperations();
  void addBranchOperations();
  void addRegMisc();
  void addReservedOperations();

  void returnSP();

  void callRetOnStack();
  void resetRetOnStack(unsigned int return_address);

  void pushToStack(uint8_t byte);
  uint8_t popFromStack();
  std::vector<uint8_t> command_args;
};

#endif  // CPU_8080_HPP_