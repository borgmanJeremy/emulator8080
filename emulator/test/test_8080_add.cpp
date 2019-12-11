#include <catch2/catch.hpp>
#include <string>
#include "cpu_8080.hpp"
#include "test_class.hpp"

TEST_CASE("Simple Adds")
{
  Cpu_8080 cpu;

  cpu.memory_.pushSegment(
    {"ram", 0x2000, smart_memory::MemoryAttribute::READ_WRITE});

  TestConfig test_scenario;
  test_scenario.addInstruction(0x80);
  test_scenario.addInstruction(0x81);
  test_scenario.addInstruction(0x82);
  test_scenario.addInstruction(0x83);
  test_scenario.addInstruction(0x84);
  test_scenario.addInstruction(0x85);
  test_scenario.addInstruction(0x86);
  test_scenario.addInstruction(0x87);
  test_scenario.addInstruction(0xC6);

  SECTION("Zero / Parity Flag Test")
  {
    test_scenario.modifyRegisterAllInstructions(REG_NAME::PC, 1,
                                                MOD_SCOPE::SOLUTION);
    test_scenario.modifyFlagAllInstructions(FLAG_NAME::Z, 1,
                                            MOD_SCOPE::SOLUTION);
    test_scenario.modifyFlagAllInstructions(FLAG_NAME::P, 1,
                                            MOD_SCOPE::SOLUTION);

    cpu.memory_[0x01] = 0x00;
    test_scenario.modifyRegisterByInstruction(REG_NAME::PC, 0x02,
                                              MOD_SCOPE::SOLUTION, 0xC6);

    for (auto const &scenario : test_scenario.test_instructions_)
    {
      SECTION(std::to_string(scenario.instruction_))
      {
        cpu.reg_ = scenario.reg_precondition_;
        cpu.flags_ = scenario.flag_precondition_;

        cpu.instruction_set_[scenario.instruction_].exp();
        INFO("The instruction is 0x"
             << std::hex << static_cast<int>(scenario.instruction_));
        REQUIRE(cpu.flags_ == scenario.flag_postcondition_);
        REQUIRE(cpu.reg_ == scenario.reg_postcondition_);
      }
    }
  }

  SECTION("Carry and Auxillary Carry Flag Test")
  {
    // Configure similar setup
    for (auto &scenario : test_scenario.test_instructions_)
    {
      scenario.reg_precondition_ = {0, 1, 1, 1, 1, 1, 1, 0, 0};
      scenario.reg_postcondition_ = {0, 1, 1, 1, 1, 1, 1, 0, 0};
    }

    test_scenario.modifyRegisterAllInstructions(REG_NAME::A, 0xFF,
                                                MOD_SCOPE::SETUP);

    cpu.memory_[0x00] = 0x01;
    cpu.memory_[0x01] = 0x01;
    test_scenario.modifyRegisterByInstruction(REG_NAME::H, 0,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x86);
    test_scenario.modifyRegisterByInstruction(REG_NAME::L, 0,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x86);

    // Configure common solutions
    test_scenario.modifyRegisterAllInstructions(REG_NAME::PC, 1,
                                                MOD_SCOPE::SOLUTION);
    test_scenario.modifyFlagAllInstructions(FLAG_NAME::CY, 1,
                                            MOD_SCOPE::SOLUTION);
    test_scenario.modifyFlagAllInstructions(FLAG_NAME::Z, 1,
                                            MOD_SCOPE::SOLUTION);
    test_scenario.modifyFlagAllInstructions(FLAG_NAME::P, 1,
                                            MOD_SCOPE::SOLUTION);
    test_scenario.modifyFlagAllInstructions(FLAG_NAME::AC, 1,
                                            MOD_SCOPE::SOLUTION);

    // Configure overrides
    // Need to override adding A to A, cant hit same flags on this as others
    test_scenario.modifyFlagByInstruction(FLAG_NAME::Z, 0, MOD_SCOPE::SOLUTION,
                                          0x87);
    test_scenario.modifyFlagByInstruction(FLAG_NAME::P, 0, MOD_SCOPE::SOLUTION,
                                          0x87);
    test_scenario.modifyFlagByInstruction(FLAG_NAME::S, 1, MOD_SCOPE::SOLUTION,
                                          0x87);
    test_scenario.modifyRegisterByInstruction(REG_NAME::A, 0xFE,
                                              MOD_SCOPE::SOLUTION, 0x87);

    test_scenario.modifyRegisterByInstruction(REG_NAME::PC, 0x02,
                                              MOD_SCOPE::SOLUTION, 0xC6);

    for (auto const &scenario : test_scenario.test_instructions_)
    {
      SECTION(std::to_string(scenario.instruction_))
      {
        cpu.reg_ = scenario.reg_precondition_;
        cpu.flags_ = scenario.flag_precondition_;

        cpu.instruction_set_[scenario.instruction_].exp();
        INFO("The instruction is 0x"
             << std::hex << static_cast<int>(scenario.instruction_));
        REQUIRE(cpu.flags_ == scenario.flag_postcondition_);
        REQUIRE(cpu.reg_ == scenario.reg_postcondition_);
      }
    }
  }

  // This test setup also checks that each addition has no side channel effects
  // on other registers
  SECTION("Test that adding from reg x does not affect reg y")
  {
    // Configure similar setup
    test_scenario.modifyRegisterAllInstructions(REG_NAME::A, 1,
                                                MOD_SCOPE::SETUP);
    test_scenario.modifyRegisterAllInstructions(REG_NAME::PC, 1,
                                                MOD_SCOPE::SOLUTION);
    test_scenario.modifyRegisterAllInstructions(REG_NAME::A, 11,
                                                MOD_SCOPE::SOLUTION);
    cpu.memory_[0x00] = 0x0A;
    cpu.memory_[0x01] = 0x0A;

    // Override specific variables
    test_scenario.modifyRegisterByInstruction(REG_NAME::B, 0x0A,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x80);
    test_scenario.modifyRegisterByInstruction(REG_NAME::C, 0x0A,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x81);
    test_scenario.modifyRegisterByInstruction(REG_NAME::D, 0x0A,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x82);
    test_scenario.modifyRegisterByInstruction(REG_NAME::E, 0x0A,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x83);
    test_scenario.modifyRegisterByInstruction(REG_NAME::H, 0x0A,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x84);
    test_scenario.modifyRegisterByInstruction(REG_NAME::L, 0x0A,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x85);

    test_scenario.modifyRegisterByInstruction(REG_NAME::H, 0x00,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x86);
    test_scenario.modifyRegisterByInstruction(REG_NAME::L, 0x00,
                                              MOD_SCOPE::SETUP_SOLUTION, 0x86);

    test_scenario.modifyRegisterByInstruction(REG_NAME::A, 0x03,
                                              MOD_SCOPE::SETUP, 0x87);
    // Override specific solutions
    test_scenario.modifyRegisterByInstruction(REG_NAME::A, 0x06,
                                              MOD_SCOPE::SOLUTION, 0x87);
    test_scenario.modifyFlagByInstruction(FLAG_NAME::P, 0x01,
                                          MOD_SCOPE::SOLUTION, 0x87);
    test_scenario.modifyRegisterByInstruction(REG_NAME::PC, 0x02,
                                              MOD_SCOPE::SOLUTION, 0xC6);

    for (auto const &scenario : test_scenario.test_instructions_)
    {
      SECTION(std::to_string(scenario.instruction_))
      {
        cpu.reg_ = scenario.reg_precondition_;
        cpu.flags_ = scenario.flag_precondition_;

        cpu.instruction_set_[scenario.instruction_].exp();
        INFO("The instruction is 0x"
             << std::hex << static_cast<int>(scenario.instruction_));
        REQUIRE(cpu.flags_ == scenario.flag_postcondition_);
        REQUIRE(cpu.reg_ == scenario.reg_postcondition_);
      }
    }
  }
}

TEST_CASE("Add ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  SECTION("Immediate")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0xC6, 0x01, 0xC6, 0x0A});

    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0xC6].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.pc == 2);

    cpu.reg_.a = 0;
    cpu.instruction_set_[0xC6].exp();
    REQUIRE(cpu.reg_.a == 10);
    REQUIRE(cpu.reg_.pc == 4);
  }
}

TEST_CASE("Add Carry Flag", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  SECTION("reg_ B")
  {
    cpu.reg_.a = 0;
    cpu.reg_.b = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x88].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.b == 0);

    cpu.reg_.a = 10;
    cpu.reg_.b = 10;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x88].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.b == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ C")
  {
    cpu.reg_.a = 0;
    cpu.reg_.c = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x89].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.c == 0);

    cpu.reg_.a = 10;
    cpu.reg_.c = 10;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x89].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.c == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ D")
  {
    cpu.reg_.a = 0;
    cpu.reg_.d = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8A].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.d == 0);

    cpu.reg_.a = 10;
    cpu.reg_.d = 10;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8A].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.d == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ E")
  {
    cpu.reg_.a = 0;
    cpu.reg_.e = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x8B].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.e == 0);

    cpu.reg_.a = 10;
    cpu.reg_.e = 10;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x8B].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.e == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ H")
  {
    cpu.reg_.a = 0;
    cpu.reg_.h = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x8C].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.h == 0);

    cpu.reg_.a = 10;
    cpu.reg_.h = 10;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8C].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.h == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ L")
  {
    cpu.reg_.a = 0;
    cpu.reg_.l = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8D].exp();
    REQUIRE(cpu.reg_.a == 1);
    REQUIRE(cpu.reg_.l == 0);

    cpu.reg_.a = 10;
    cpu.reg_.l = 10;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8D].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.l == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ A")
  {
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8F].exp();
    REQUIRE(cpu.reg_.a == 1);

    cpu.reg_.a = 10;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8F].exp();
    REQUIRE(cpu.reg_.a == 21);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("Data")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xFF, 0x01, 0xFE});
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0xCE].exp();
    REQUIRE(cpu.reg_.pc == 0x02);
    REQUIRE(cpu.reg_.a == 0x00);
  }

  SECTION("Memory")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xFF, 0x01, 0xFE});
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8E].exp();
    REQUIRE(cpu.reg_.a == 1);

    cpu.reg_.a = 0;
    cpu.reg_.h = 0;
    cpu.reg_.l = 1;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8E].exp();
    REQUIRE(cpu.reg_.a == 0x00);

    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0;
    cpu.reg_.l = 2;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x8E].exp();
    REQUIRE(cpu.reg_.a == 0x01);

    REQUIRE(cpu.reg_.pc == 3);
  }
}

TEST_CASE("Double Add", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;

  SECTION("reg BC")
  {
    cpu.flags_.cy = 0;

    cpu.reg_.h = 0x01;
    cpu.reg_.l = 0xFF;

    cpu.reg_.b = 0x01;
    cpu.reg_.c = 0x01;

    cpu.instruction_set_[0x09].exp();
    REQUIRE(cpu.reg_.h == 0x03);
    REQUIRE(cpu.reg_.l == 0x00);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 0x01);

    cpu.flags_.cy = 0;

    cpu.reg_.h = 0xFF;
    cpu.reg_.l = 0xFF;

    cpu.reg_.b = 0x00;
    cpu.reg_.c = 0x01;

    cpu.instruction_set_[0x09].exp();
    REQUIRE(cpu.reg_.h == 0x00);
    REQUIRE(cpu.reg_.l == 0x00);
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("reg DE")
  {
    cpu.flags_.cy = 0;

    cpu.reg_.h = 0x01;
    cpu.reg_.l = 0xFF;

    cpu.reg_.d = 0x01;
    cpu.reg_.e = 0x01;

    cpu.instruction_set_[0x19].exp();
    REQUIRE(cpu.reg_.h == 0x03);
    REQUIRE(cpu.reg_.l == 0x00);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg HL")
  {
    cpu.flags_.cy = 0;

    cpu.reg_.h = 0x01;
    cpu.reg_.l = 0xFF;

    cpu.instruction_set_[0x29].exp();
    REQUIRE(cpu.reg_.h == 0x03);
    REQUIRE(cpu.reg_.l == 0xFE);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg SP")
  {
    cpu.flags_.cy = 0;

    cpu.reg_.h = 0x01;
    cpu.reg_.l = 0xFF;

    cpu.reg_.sp = 0x0101;

    cpu.instruction_set_[0x39].exp();
    REQUIRE(cpu.reg_.h == 0x03);
    REQUIRE(cpu.reg_.l == 0x00);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}

TEST_CASE("DAA")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 0x9B;
  cpu.flags_.ac = 0;
  cpu.flags_.cy = 0;

  cpu.instruction_set_[0x27].exp();
  REQUIRE(cpu.reg_.pc == 1);
  REQUIRE(cpu.reg_.a == 0x01);
  REQUIRE(cpu.flags_.ac == 1);
  REQUIRE(cpu.flags_.cy == 1);
}