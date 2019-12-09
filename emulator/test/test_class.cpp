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
                            RegisterSolution &elem)
{
  switch (flag)
  {
    case FLAG_NAME::AC:
    {
      modifyValuesInScope(elem.flag_setup_.ac, elem.flag_solution_.ac, value,
                          mod);
      break;
    }
    case FLAG_NAME::CY:
    {
      modifyValuesInScope(elem.flag_setup_.cy, elem.flag_solution_.cy, value,
                          mod);
      break;
    }

    case FLAG_NAME::P:
    {
      modifyValuesInScope(elem.flag_setup_.p, elem.flag_solution_.p, value,
                          mod);
      break;
    }

    case FLAG_NAME::S:
    {
      modifyValuesInScope(elem.flag_setup_.s, elem.flag_solution_.s, value,
                          mod);
      break;
    }
    case FLAG_NAME::Z:
    {
      modifyValuesInScope(elem.flag_setup_.z, elem.flag_solution_.z, value,
                          mod);
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

void TestConfig::modifyRegister(REG_NAME reg, unsigned int value, MOD_SCOPE mod,
                                RegisterSolution &elem)
{
  switch (reg)
  {
    case (REG_NAME::A):
    {
      modifyValuesInScope(elem.reg_setup_.a, elem.reg_solution_.a, value, mod);
      break;
    }
    case (REG_NAME::B):
    {
      modifyValuesInScope(elem.reg_setup_.b, elem.reg_solution_.b, value, mod);
      break;
    }
    case (REG_NAME::C):
    {
      modifyValuesInScope(elem.reg_setup_.c, elem.reg_solution_.c, value, mod);
      break;
    }
    case (REG_NAME::D):
    {
      modifyValuesInScope(elem.reg_setup_.d, elem.reg_solution_.d, value, mod);
      break;
    }
    case (REG_NAME::E):
    {
      modifyValuesInScope(elem.reg_setup_.e, elem.reg_solution_.e, value, mod);
      break;
    }
    case (REG_NAME::H):
    {
      modifyValuesInScope(elem.reg_setup_.h, elem.reg_solution_.h, value, mod);
      break;
    }
    case (REG_NAME::L):
    {
      modifyValuesInScope(elem.reg_setup_.l, elem.reg_solution_.l, value, mod);
      break;
    }
    case (REG_NAME::SP):
    {
      modifyValuesInScope(elem.reg_setup_.sp, elem.reg_solution_.sp, value,
                          mod);
      break;
    }
    case (REG_NAME::PC):
    {
      modifyValuesInScope(elem.reg_setup_.pc, elem.reg_solution_.pc, value,
                          mod);
      break;
    }
  }
}
