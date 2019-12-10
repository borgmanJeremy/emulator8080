#include <string>
#include <vector>
#include "cpu_8080.hpp"

#ifndef TEST_CLASS_HPP_
#define TEST_CLASS_HPP_

enum class MOD_SCOPE
{
  SOLUTION,
  SETUP,
  SETUP_SOLUTION
};

enum class FLAG_NAME
{
  Z,
  S,
  P,
  CY,
  AC
};

enum class REG_NAME
{
  A,
  B,
  C,
  D,
  E,
  H,
  L,
  SP,
  PC
};

struct ScenarioGroup
{
  uint8_t instruction_;
  Registers reg_precondition_;
  Registers reg_postcondition_;
  Flags flag_precondition_;
  Flags flag_postcondition_;
};

class TestConfig
{
 public:
  TestConfig() = default;

  void addInstruction(uint8_t instruction);
  void modifyRegisterAllInstructions(REG_NAME reg, unsigned int value,
                                     MOD_SCOPE mod);

  void modifyRegisterByInstruction(REG_NAME reg, unsigned int value,
                                   MOD_SCOPE mod, uint8_t instruction);

  void modifyFlagAllInstructions(FLAG_NAME flag, unsigned int value,
                                 MOD_SCOPE mod);

  void modifyFlagByInstruction(FLAG_NAME flag, unsigned int value,
                               MOD_SCOPE mod, uint8_t instruction);

  Flags default_flags_{0, 0, 0, 0, 0};
  Registers default_registers_{0, 0, 0, 0, 0, 0, 0, 0, 0};

  std::vector<ScenarioGroup> test_instructions_;

 private:
  void modifyValuesInScope(uint8_t &setup, uint8_t &solution, uint8_t value,
                           MOD_SCOPE mod);

  void modifyValuesInScope(uint16_t &setup, uint16_t &solution, uint16_t value,
                           MOD_SCOPE mod);

  void modifyRegister(REG_NAME reg, unsigned int value, MOD_SCOPE mod,
                      ScenarioGroup &elem);

  void modifyFlag(FLAG_NAME flag, unsigned int value, MOD_SCOPE mod,
                  ScenarioGroup &elem);
};

#endif  // TEST_CLASS_HPP_