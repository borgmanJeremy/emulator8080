#include <catch2/catch.hpp>
#include "cpu_8080.hpp"
TEST_CASE("Math Flags", "[flag]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  SECTION("Zero")
  {
    cpu.reg_.a = 0;
    cpu.reg_.b = 0;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.z == 1);

    cpu.reg_.a = 1;
    cpu.reg_.b = 0;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.z == 0);

    cpu.reg_.a = 1;
    cpu.reg_.b = 0xFF;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.z == 1);

    cpu.reg_.a = 1;
    cpu.reg_.b = -1;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.z == 1);
  }

  SECTION("Sign")
  {
    cpu.reg_.a = 1;
    cpu.reg_.b = 0;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.s == 0);

    cpu.reg_.a = 0;
    cpu.reg_.b = 0x80;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.s == 1);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 1;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.s == 0);

    cpu.reg_.a = 0;
    cpu.reg_.b = 0x01;
    cpu.instruction_set_[0x90].exp();
    REQUIRE(cpu.flags_.s == 1);
  }

  SECTION("Carry")
  {
    cpu.reg_.a = 1;
    cpu.reg_.b = 1;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0xFF;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.cy == 1);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0x01;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.cy == 1);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0x00;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0xFE;
    cpu.reg_.b = 0x01;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0;
    cpu.reg_.b = 0x01;
    cpu.instruction_set_[0x90].exp();
    REQUIRE(cpu.flags_.cy == 1);
  }

  SECTION("Parity")
  {
    // 1+2 = 3 (b011)
    cpu.reg_.a = 1;
    cpu.reg_.b = 2;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.p == 1);

    cpu.reg_.a = 1;
    cpu.reg_.b = 0;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.p == 0);

    cpu.reg_.a = 0;
    cpu.reg_.b = 0xFF;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.p == 1);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 2;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.p == 0);

    cpu.reg_.a = 0x00;
    cpu.reg_.b = 0x00;
    cpu.instruction_set_[0x80].exp();
    REQUIRE(cpu.flags_.p == 1);
  }
}