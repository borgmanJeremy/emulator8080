#include <catch2/catch.hpp>
#include <map>
#include <string>
#include "cpu_8080.hpp"

struct RegisterSolution
{
  uint8_t instruction_;
  Registers reg_solution_;
  Flags flag_solution_;
};

TEST_CASE("Simple Adds")
{
  Cpu_8080 cpu;

  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  Flags common_flag_solution{0, 0, 0, 0, 0};
  Registers common_register_solution{0, 0, 0, 0, 0, 0, 0, 0, 0};

  std::vector<RegisterSolution> test_instructions;
  test_instructions.resize(2);
  std::fill(
    test_instructions.begin(), test_instructions.end(),
    RegisterSolution{0x00, common_register_solution, common_flag_solution});

  test_instructions[0].instruction_ = 0x80;
  test_instructions[1].instruction_ = 0x81;

  SECTION("Result is zero, z=1 s=0 p=1 ac=0 cy=0")
  {
    for (auto &result : test_instructions)
    {
      result.flag_solution_.z = 1;
      result.flag_solution_.p = 1;
      result.reg_solution_.pc = 1;
    }

    for (auto const &instruction : test_instructions)
    {
      SECTION(std::to_string(instruction.instruction_))
      {
        cpu.instruction_set_[instruction.instruction_].exp();
        INFO("The instruction is "
             << static_cast<int>(instruction.instruction_));
        REQUIRE(cpu.flags_ == instruction.flag_solution_);
        REQUIRE(cpu.reg_ == instruction.reg_solution_);
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

  SECTION("reg_ B")
  {
    cpu.reg_.a = 0;
    cpu.reg_.b = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.b == 0);

    cpu.reg_.a = 10;
    cpu.reg_.b = 10;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.b == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ C")
  {
    cpu.reg_.a = 0;
    cpu.reg_.c = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x81].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.c == 0);

    cpu.reg_.a = 10;
    cpu.reg_.c = 10;
    cpu.instruction_set_[0x81].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.c == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ D")
  {
    cpu.reg_.a = 0;
    cpu.reg_.d = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x82].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.d == 0);

    cpu.reg_.a = 10;
    cpu.reg_.d = 10;
    cpu.instruction_set_[0x82].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.d == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ E")
  {
    cpu.reg_.a = 0;
    cpu.reg_.e = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x83].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.e == 0);

    cpu.reg_.a = 10;
    cpu.reg_.e = 10;
    cpu.instruction_set_[0x83].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.e == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ H")
  {
    cpu.reg_.a = 0;
    cpu.reg_.h = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x84].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.h == 0);

    cpu.reg_.a = 10;
    cpu.reg_.h = 10;
    cpu.instruction_set_[0x84].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.h == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ L")
  {
    cpu.reg_.a = 0;
    cpu.reg_.l = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x85].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.l == 0);

    cpu.reg_.a = 10;
    cpu.reg_.l = 10;
    cpu.instruction_set_[0x85].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.l == 10);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ A")
  {
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x87].exp();
    REQUIRE(cpu.reg_.a == 0);

    cpu.reg_.a = 10;
    cpu.instruction_set_[0x87].exp();
    REQUIRE(cpu.reg_.a == 20);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("Memory")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xFF, 0x01, 0xFE});
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.reg_.h = 0;
    cpu.reg_.l = 0;

    cpu.instruction_set_[0x86].exp();
    REQUIRE(cpu.reg_.a == 0);

    cpu.reg_.a = 0;
    cpu.reg_.h = 0;
    cpu.reg_.l = 1;
    cpu.instruction_set_[0x86].exp();
    REQUIRE(cpu.reg_.a == 0xFF);

    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0;
    cpu.reg_.l = 2;
    cpu.instruction_set_[0x86].exp();
    REQUIRE(cpu.reg_.a == 0x00);

    REQUIRE(cpu.reg_.pc == 3);
  }

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