#include "test_class.hpp"

void TestConfig::addInstruction(uint8_t instruction)
{
  test_instructions_.push_back({instruction, default_registers_,
                                default_registers_, default_flags_,
                                default_flags_});
}

void TestConfig::modifyValuesInScope(uint8_t &setup, uint8_t &solution,
                                     uint8_t value, MOD_SCOPE mod)
{
  switch (mod)
  {
    case MOD_SCOPE::SETUP:
    {
      setup = value;
      break;
    }
    case MOD_SCOPE::SOLUTION:
    {
      solution = value;
      break;
    }
    case MOD_SCOPE::SETUP_SOLUTION:
    {
      setup = value;
      solution = value;
      break;
    }
  }
}

void TestConfig::modifyValuesInScope(uint16_t &setup, uint16_t &solution,
                                     uint16_t value, MOD_SCOPE mod)
{
  switch (mod)
  {
    case MOD_SCOPE::SETUP:
    {
      setup = value;
      break;
    }
    case MOD_SCOPE::SOLUTION:
    {
      solution = value;
      break;
    }
    case MOD_SCOPE::SETUP_SOLUTION:
    {
      setup = value;
      solution = value;
      break;
    }
  }
}

void TestConfig::modifyFlag(FLAG_NAME flag, unsigned int value, MOD_SCOPE mod,
                            ScenarioGroup &elem)
{
  switch (flag)
  {
    case FLAG_NAME::AC:
    {
      modifyValuesInScope(elem.flag_precondition_.ac,
                          elem.flag_postcondition_.ac, value, mod);
      break;
    }
    case FLAG_NAME::CY:
    {
      modifyValuesInScope(elem.flag_precondition_.cy,
                          elem.flag_postcondition_.cy, value, mod);
      break;
    }

    case FLAG_NAME::P:
    {
      modifyValuesInScope(elem.flag_precondition_.p, elem.flag_postcondition_.p,
                          value, mod);
      break;
    }

    case FLAG_NAME::S:
    {
      modifyValuesInScope(elem.flag_precondition_.s, elem.flag_postcondition_.s,
                          value, mod);
      break;
    }
    case FLAG_NAME::Z:
    {
      modifyValuesInScope(elem.flag_precondition_.z, elem.flag_postcondition_.z,
                          value, mod);
      break;
    }
  }
}

void TestConfig::modifyFlagByInstruction(FLAG_NAME flag, unsigned int value,
                                         MOD_SCOPE mod, uint8_t instruction)
{
  for (auto &elem : test_instructions_)
  {
    if (elem.instruction_ == instruction)
    {
      modifyFlag(flag, value, mod, elem);
    }
  }
}

void TestConfig::modifyFlagAllInstructions(FLAG_NAME flag, unsigned int value,
                                           MOD_SCOPE mod)
{
  for (auto &elem : test_instructions_)
  {
    modifyFlag(flag, value, mod, elem);
  }
}

void TestConfig::modifyRegisterByInstruction(REG_NAME reg, unsigned int value,
                                             MOD_SCOPE mod, uint8_t instruction)
{
  for (auto &elem : test_instructions_)
  {
    if (elem.instruction_ == instruction)
    {
      modifyRegister(reg, value, mod, elem);
    }
  }
}

void TestConfig::modifyRegisterAllInstructions(REG_NAME reg, unsigned int value,
                                               MOD_SCOPE mod)
{
  for (auto &instruction : test_instructions_)
  {
    modifyRegister(reg, value, mod, instruction);
  }
}

void TestConfig::modifyCycleCountAllInstructions(unsigned int value)
{
  for (auto &instruction : test_instructions_)
  {
    instruction.cycle_count_postcondition_ = value;
  }
}
void TestConfig::modifyCycleCountByInstruction(unsigned int value,
                                               uint8_t instruction)
{
  for (auto &elem : test_instructions_)
  {
    if (elem.instruction_ == instruction)
    {
      elem.cycle_count_postcondition_ = value;
    }
  }
}

void TestConfig::modifyRegister(REG_NAME reg, unsigned int value, MOD_SCOPE mod,
                                ScenarioGroup &elem)
{
  switch (reg)
  {
    case (REG_NAME::A):
    {
      modifyValuesInScope(elem.reg_precondition_.a, elem.reg_postcondition_.a,
                          value, mod);
      break;
    }
    case (REG_NAME::B):
    {
      modifyValuesInScope(elem.reg_precondition_.b, elem.reg_postcondition_.b,
                          value, mod);
      break;
    }
    case (REG_NAME::C):
    {
      modifyValuesInScope(elem.reg_precondition_.c, elem.reg_postcondition_.c,
                          value, mod);
      break;
    }
    case (REG_NAME::D):
    {
      modifyValuesInScope(elem.reg_precondition_.d, elem.reg_postcondition_.d,
                          value, mod);
      break;
    }
    case (REG_NAME::E):
    {
      modifyValuesInScope(elem.reg_precondition_.e, elem.reg_postcondition_.e,
                          value, mod);
      break;
    }
    case (REG_NAME::H):
    {
      modifyValuesInScope(elem.reg_precondition_.h, elem.reg_postcondition_.h,
                          value, mod);
      break;
    }
    case (REG_NAME::L):
    {
      modifyValuesInScope(elem.reg_precondition_.l, elem.reg_postcondition_.l,
                          value, mod);
      break;
    }
    case (REG_NAME::SP):
    {
      modifyValuesInScope(elem.reg_precondition_.sp, elem.reg_postcondition_.sp,
                          value, mod);
      break;
    }
    case (REG_NAME::PC):
    {
      modifyValuesInScope(elem.reg_precondition_.pc, elem.reg_postcondition_.pc,
                          value, mod);
      break;
    }
  }
}
