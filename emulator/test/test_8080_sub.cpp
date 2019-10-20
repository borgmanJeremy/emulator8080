#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("SUB ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  SECTION("Reg B")
  {
    cpu.reg_.a = 0;
    cpu.reg_.b = 0;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x90].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.b == 0);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 10;
    cpu.reg_.b = 5;
    cpu.instruction_set_[0x90].exp();
    REQUIRE(cpu.reg_.a == 5);
    REQUIRE(cpu.reg_.b == 5);
    REQUIRE(cpu.reg_.pc == 2);

    cpu.reg_.a = 0;
    cpu.reg_.b = 1;
    cpu.instruction_set_[0x90].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.b == 1);
    REQUIRE(cpu.reg_.pc == 3);
  }

  SECTION("Reg C")
  {
    cpu.reg_.a = 10;
    cpu.reg_.c = 5;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x91].exp();
    REQUIRE(cpu.reg_.a == 5);
    REQUIRE(cpu.reg_.c == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.c = 1;
    cpu.instruction_set_[0x91].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.c == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("Reg D")
  {
    cpu.reg_.a = 10;
    cpu.reg_.d = 5;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x92].exp();
    REQUIRE(cpu.reg_.a == 5);
    REQUIRE(cpu.reg_.d == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.d = 1;
    cpu.instruction_set_[0x92].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.d == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg E")
  {
    cpu.reg_.a = 10;
    cpu.reg_.e = 5;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x93].exp();
    REQUIRE(cpu.reg_.a == 5);
    REQUIRE(cpu.reg_.e == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.e = 1;
    cpu.instruction_set_[0x93].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.e == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg H")
  {
    cpu.reg_.a = 10;
    cpu.reg_.h = 5;
    cpu.reg_.pc = 0;

    cpu.instruction_set_[0x94].exp();
    REQUIRE(cpu.reg_.a == 5);
    REQUIRE(cpu.reg_.h == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.h = 1;
    cpu.instruction_set_[0x94].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.h == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg L")
  {
    cpu.reg_.a = 10;
    cpu.reg_.l = 5;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x95].exp();
    REQUIRE(cpu.reg_.a == 5);
    REQUIRE(cpu.reg_.l == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.l = 1;
    cpu.instruction_set_[0x95].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.l == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg A")
  {
    cpu.reg_.a = 10;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x97].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("Data")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0x10, 0x01, 0xFE});
    cpu.reg_.a = 0x10;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0xD6].exp();
    REQUIRE(cpu.reg_.a == 0);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("Memory")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x01, 0xFF, 0x01, 0xFE});
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.reg_.h = 0;
    cpu.reg_.l = 0;

    cpu.instruction_set_[0x96].exp();
    REQUIRE(cpu.reg_.a == 0xFF);

    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0;
    cpu.reg_.l = 1;
    cpu.instruction_set_[0x96].exp();
    REQUIRE(cpu.reg_.a == 0x00);

    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0;
    cpu.reg_.l = 2;
    cpu.instruction_set_[0x96].exp();
    REQUIRE(cpu.reg_.a == 0xFE);

    REQUIRE(cpu.reg_.pc == 3);
  }
}

TEST_CASE("SUB Borrow", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  SECTION("Reg B")
  {
    cpu.reg_.a = 0;
    cpu.reg_.b = 0;
    cpu.flags_.cy = 1;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x98].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.b == 0);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 10;
    cpu.reg_.b = 5;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x98].exp();
    REQUIRE(cpu.reg_.a == 4);
    REQUIRE(cpu.reg_.b == 5);
    REQUIRE(cpu.reg_.pc == 2);

    cpu.reg_.a = 0;
    cpu.reg_.b = 1;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x98].exp();
    REQUIRE(cpu.reg_.a == 0xFE);
    REQUIRE(cpu.reg_.b == 1);
    REQUIRE(cpu.reg_.pc == 3);
  }

  SECTION("Reg C")
  {
    cpu.reg_.a = 10;
    cpu.reg_.c = 5;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x99].exp();
    REQUIRE(cpu.reg_.a == 4);
    REQUIRE(cpu.reg_.c == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.c = 1;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x99].exp();
    REQUIRE(cpu.reg_.a == 0xFE);
    REQUIRE(cpu.reg_.c == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("Reg D")
  {
    cpu.reg_.a = 10;
    cpu.reg_.d = 5;
    cpu.flags_.cy = 1;
    cpu.reg_.pc = 0;

    cpu.instruction_set_[0x9A].exp();
    REQUIRE(cpu.reg_.a == 4);
    REQUIRE(cpu.reg_.d == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.d = 1;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x9A].exp();
    REQUIRE(cpu.reg_.a == 0xFE);
    REQUIRE(cpu.reg_.d == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg E")
  {
    cpu.reg_.a = 10;
    cpu.reg_.e = 5;
    cpu.flags_.cy = 1;
    cpu.reg_.pc = 0;

    cpu.instruction_set_[0x9B].exp();
    REQUIRE(cpu.reg_.a == 4);
    REQUIRE(cpu.reg_.e == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.e = 1;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x9B].exp();
    REQUIRE(cpu.reg_.a == 0xFE);
    REQUIRE(cpu.reg_.e == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg H")
  {
    cpu.reg_.a = 10;
    cpu.reg_.h = 5;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x9C].exp();
    REQUIRE(cpu.reg_.a == 4);
    REQUIRE(cpu.reg_.h == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.h = 1;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x9C].exp();
    REQUIRE(cpu.reg_.a == 0xFE);
    REQUIRE(cpu.reg_.h == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg L")
  {
    cpu.reg_.a = 10;
    cpu.reg_.l = 5;
    cpu.flags_.cy = 1;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x9D].exp();
    REQUIRE(cpu.reg_.a == 4);
    REQUIRE(cpu.reg_.l == 5);
    REQUIRE(cpu.reg_.pc == 1);

    cpu.reg_.a = 0;
    cpu.reg_.l = 1;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x9D].exp();
    REQUIRE(cpu.reg_.a == 0xFE);
    REQUIRE(cpu.reg_.l == 1);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("Reg A")
  {
    cpu.reg_.a = 10;
    cpu.flags_.cy = 1;
    cpu.reg_.pc = 0;
    cpu.instruction_set_[0x9F].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("Memory")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xFE});
    cpu.reg_.a = 0xFF;
    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0xDE].exp();
    REQUIRE(cpu.reg_.a == 0x00);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("Memory")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x01, 0xFF, 0x01, 0xFE});
    cpu.reg_.a = 0;
    cpu.reg_.pc = 0;
    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0x9E].exp();
    REQUIRE(cpu.reg_.a == 0xFE);

    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0;
    cpu.reg_.l = 1;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x9E].exp();
    REQUIRE(cpu.reg_.a == 0xFF);

    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0;
    cpu.reg_.l = 2;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0x9E].exp();
    REQUIRE(cpu.reg_.a == 0xFD);

    REQUIRE(cpu.reg_.pc == 3);
  }
}

TEST_CASE("Decrement Double ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;

  SECTION("DCX B")
  {
    cpu.reg_.b = 0x01;
    cpu.reg_.c = 0x00;

    cpu.instruction_set_[0x0B].exp();
    REQUIRE(cpu.reg_.b == 0x00);
    REQUIRE(cpu.reg_.c == 0xFF);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("DCX D")
  {
    cpu.reg_.d = 0x01;
    cpu.reg_.e = 0x00;

    cpu.instruction_set_[0x1B].exp();
    REQUIRE(cpu.reg_.d == 0x00);
    REQUIRE(cpu.reg_.e == 0xFF);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("DCX HL")
  {
    cpu.reg_.h = 0x01;
    cpu.reg_.l = 0x00;

    cpu.instruction_set_[0x2B].exp();
    REQUIRE(cpu.reg_.h == 0x00);
    REQUIRE(cpu.reg_.l == 0xFF);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("DCX SP")
  {
    cpu.reg_.sp = 0x0100;

    cpu.instruction_set_[0x3B].exp();
    REQUIRE(cpu.reg_.sp == 0xFF);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}