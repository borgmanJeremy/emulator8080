#include "cpu_8080.hpp"

Cpu_8080::Cpu_8080()
{
  int_enable_ = 0;
  // cycle_count_.setLimit(100'000);

  port_.resize(port_size_);
  std::fill(port_.begin(), port_.end(), 0);
  port_[0] = 0x0E;
  port_[1] = 0x09;

  instruction_set_.emplace_back(Instruction{0, 0, "NOP", [this]() {
                                              cycle_count_ += 4;
                                              reg_.pc += 1;
                                            }});

  instruction_set_.emplace_back(Instruction{
    0x76, 0, "HLT",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  addMathAddOperations();
  addMathSubOperations();
  addMoveOperations();
  addLogicalOperations();
  addBranchOperations();
  addRegMisc();
  addReservedOperations();

  std::sort(instruction_set_.begin(), instruction_set_.end(),
            [](Instruction const &a, Instruction const &b) {
              return a.op_code < b.op_code;
            });

  // print instruction set
  // for (size_t i = 0; i < instruction_set_.size(); i++)
  // {
  //   std::cout << i << "\t" << std::to_string(instruction_set[i].op_code) <<
  //   "\t"
  //             << instruction_set[i].instruction << "\t"
  //             << std::to_string(instruction_set[i].num_args) << std::endl;
  // }
}

// This only checks the forth bit, so to correctly work the upper 4 bits should
// be masked to zero before math operations
void Cpu_8080::setAuxFlag(uint8_t result)
{
  if (result & (0x10))
  {
    flags_.ac = 1;
  }
  else
  {
    flags_.ac = 1;
  }
}

void Cpu_8080::setZeroFlag(uint16_t result)
{
  // On real 8 bit hardware I think 0xFF + 0x01 would equal 0x00. Since
  // I am casting to 16 bit for easier testing, need to emulate this
  // behavior by only looking at lower 8 bits.
  if ((result & 0xFF) == 0)
  {
    flags_.z = 1;
  }
  else
  {
    flags_.z = 0;
  }
}
void Cpu_8080::setSignFlag(uint16_t result)
{
  if (result & 0x80)
  {
    flags_.s = 1;
  }

  else
  {
    flags_.s = 0;
  }
}
void Cpu_8080::setCarryFlag(uint16_t result)
{
  if (result > 0xFF)
  {
    flags_.cy = 1;
  }

  else
  {
    flags_.cy = 0;
  }
}
void Cpu_8080::setParityFlag(uint16_t result)
{
  unsigned int count = 0;
  for (unsigned int i = 1; i <= 0x80; i *= 2)
  {
    if (result & i)
    {
      count += 1;
    }
  }

  // Mod 2 = 0 means even
  // TODO: Add test for parity when number is 0, NOT COVERED yet
  if (!(count % 2))
  {
    flags_.p = 1;
  }
  else
  {
    flags_.p = 0;
  }
}

uint16_t Cpu_8080::getAddressFromHL() const
{
  return static_cast<uint16_t>((static_cast<uint16_t>(reg_.h) << 8U) + reg_.l);
}

uint16_t Cpu_8080::getAddressFromBC() const
{
  return static_cast<uint16_t>((static_cast<uint16_t>(reg_.b) << 8U) + reg_.c);
}

uint16_t Cpu_8080::getAddressFromDE() const
{
  return static_cast<uint16_t>((static_cast<uint16_t>(reg_.d) << 8U) + reg_.e);
}

uint16_t Cpu_8080::getAddressFromTwoBytes(unsigned int base_address) const
{
  return static_cast<uint16_t>(
    (static_cast<uint16_t>(memory_[base_address + 1]) << 8U) +
    memory_[base_address]);
}