#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("Misc Swaps")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  constexpr unsigned int stack_start = 0x2200;
  cpu.reg_.pc = 0;
  cpu.reg_.sp = stack_start;

  SECTION("XTHL")
  {
    cpu.reg_.l = 0xAA;
    cpu.reg_.h = 0x55;
    cpu.memory_[cpu.reg_.sp] = 0xEE;
    cpu.memory_[cpu.reg_.sp + 1] = 0xFF;

    cpu.instruction_set_[0xE3].exp();
    REQUIRE(cpu.reg_.l == 0xEE);
    REQUIRE(cpu.reg_.h == 0xFF);
    REQUIRE(cpu.memory_[cpu.reg_.sp] == 0xAA);
    REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0x55);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("XCHG")
  {
    cpu.reg_.d = 0x11;
    cpu.reg_.e = 0x22;

    cpu.reg_.h = 0xAA;
    cpu.reg_.l = 0x55;

    cpu.instruction_set_[0xEB].exp();
    REQUIRE(cpu.reg_.pc == 0x01);

    REQUIRE(cpu.reg_.d == 0xAA);
    REQUIRE(cpu.reg_.e == 0x55);
    REQUIRE(cpu.reg_.h == 0x11);
    REQUIRE(cpu.reg_.l == 0x22);
  }

  SECTION("SPHL")
  {
    cpu.reg_.h = 0x55;
    cpu.reg_.l = 0xAA;

    cpu.instruction_set_[0xF9].exp();

    REQUIRE(cpu.reg_.sp == 0x55AA);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}

TEST_CASE("PUSH ", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  constexpr unsigned int stack_start = 0x2200;
  cpu.reg_.pc = 0;
  cpu.reg_.sp = stack_start;

  SECTION("B")
  {
    cpu.reg_.b = 0xAA;
    cpu.reg_.c = 0x55;
    cpu.instruction_set_[0xC5].exp();

    REQUIRE(cpu.reg_.sp == stack_start - 2);
    REQUIRE(cpu.memory_[cpu.reg_.sp] == 0x55);
    REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0xAA);
  }

  SECTION("D")
  {
    cpu.reg_.d = 0xAA;
    cpu.reg_.e = 0x55;
    cpu.instruction_set_[0xD5].exp();

    REQUIRE(cpu.reg_.sp == stack_start - 2);
    REQUIRE(cpu.memory_[cpu.reg_.sp] == 0x55);
    REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0xAA);
  }

  SECTION("H")
  {
    cpu.reg_.h = 0xAA;
    cpu.reg_.l = 0x55;
    cpu.instruction_set_[0xE5].exp();

    REQUIRE(cpu.reg_.sp == stack_start - 2);
    REQUIRE(cpu.memory_[cpu.reg_.sp] == 0x55);
    REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0xAA);
  }
  SECTION("PSW")
  {
    cpu.reg_.a = 0x55;
    cpu.flags_.z = 1;
    cpu.flags_.s = 0;
    cpu.flags_.p = 1;
    cpu.flags_.cy = 0;
    cpu.flags_.ac = 1;

    cpu.instruction_set_[0xF5].exp();
    REQUIRE(cpu.reg_.sp == stack_start - 2);
    REQUIRE(cpu.memory_[cpu.reg_.sp] == 0x15);
    REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0x55);
  }

  // TODO Move common checks to end like this
  REQUIRE(cpu.reg_.pc == 1);
}
TEST_CASE("POP ", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  // TODO: fix magic numbers like stack starting address
  cpu.reg_.sp = 0x2200 - 2;
  cpu.memory_[0x2200 - 1] = 0xAA;
  cpu.memory_[0x2200 - 2] = 0x55;

  SECTION("B")
  {
    cpu.instruction_set_[0xC1].exp();
    REQUIRE(cpu.reg_.b == 0xAA);
    REQUIRE(cpu.reg_.c == 0x55);
    REQUIRE(cpu.reg_.sp == 0x2200);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("D")
  {
    cpu.instruction_set_[0xD1].exp();
    REQUIRE(cpu.reg_.d == 0xAA);
    REQUIRE(cpu.reg_.e == 0x55);
    REQUIRE(cpu.reg_.sp == 0x2200);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("H")
  {
    cpu.instruction_set_[0xE1].exp();
    REQUIRE(cpu.reg_.h == 0xAA);
    REQUIRE(cpu.reg_.l == 0x55);
    REQUIRE(cpu.reg_.sp == 0x2200);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  // TODO: Better test to make sure all masks work
  SECTION("PSW")
  {
    cpu.instruction_set_[0xF1].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.z == 0x01);
    REQUIRE(cpu.flags_.s == 0x00);
    REQUIRE(cpu.flags_.p == 0x01);
    REQUIRE(cpu.flags_.cy == 0x00);
    REQUIRE(cpu.flags_.ac == 0x01);

    REQUIRE(cpu.reg_.sp == 0x2200);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}

TEST_CASE("Mov B", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    cpu.instruction_set_[0x40].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    solution.b = 3;
    cpu.instruction_set_[0x41].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    solution.b = 4;
    cpu.instruction_set_[0x42].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    solution.b = 5;
    cpu.instruction_set_[0x43].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    solution.b = 6;
    cpu.instruction_set_[0x44].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    solution.b = 7;
    cpu.instruction_set_[0x45].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.b = 0x42;
    solution.h = 0;
    solution.l = 0;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x46].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    solution.b = 1;
    cpu.instruction_set_[0x47].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov C", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    solution.c = 2;
    cpu.instruction_set_[0x48].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    cpu.instruction_set_[0x49].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    solution.c = 4;
    cpu.instruction_set_[0x4A].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    solution.c = 5;
    cpu.instruction_set_[0x4B].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    solution.c = 6;
    cpu.instruction_set_[0x4C].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    solution.c = 7;
    cpu.instruction_set_[0x4D].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.c = 0x42;
    solution.h = 0;
    solution.l = 0;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x4E].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    solution.c = 1;
    cpu.instruction_set_[0x4F].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov D", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    solution.d = 2;
    cpu.instruction_set_[0x50].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    solution.d = 3;
    cpu.instruction_set_[0x51].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    cpu.instruction_set_[0x52].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    solution.d = 5;
    cpu.instruction_set_[0x53].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    solution.d = 6;
    cpu.instruction_set_[0x54].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    solution.d = 7;
    cpu.instruction_set_[0x55].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.d = 0x42;
    solution.h = 0;
    solution.l = 0;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x56].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    solution.d = 1;
    cpu.instruction_set_[0x57].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov E", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    solution.e = 2;
    cpu.instruction_set_[0x58].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    solution.e = 3;
    cpu.instruction_set_[0x59].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    solution.e = 4;

    cpu.instruction_set_[0x5A].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    cpu.instruction_set_[0x5B].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    solution.e = 6;
    cpu.instruction_set_[0x5C].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    solution.e = 7;
    cpu.instruction_set_[0x5D].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.e = 0x42;
    solution.h = 0;
    solution.l = 0;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x5E].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    solution.e = 1;
    cpu.instruction_set_[0x5F].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov H", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    solution.h = 2;
    cpu.instruction_set_[0x60].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    solution.h = 3;
    cpu.instruction_set_[0x61].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    solution.h = 4;

    cpu.instruction_set_[0x62].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    solution.h = 5;
    cpu.instruction_set_[0x63].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    cpu.instruction_set_[0x64].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    solution.h = 7;
    cpu.instruction_set_[0x65].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.h = 0x42;
    solution.l = 0;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x66].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    solution.h = 1;
    cpu.instruction_set_[0x67].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov L", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    solution.l = 2;
    cpu.instruction_set_[0x68].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    solution.l = 3;
    cpu.instruction_set_[0x69].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    solution.l = 4;

    cpu.instruction_set_[0x6A].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    solution.l = 5;
    cpu.instruction_set_[0x6B].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    solution.l = 6;
    cpu.instruction_set_[0x6C].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    cpu.instruction_set_[0x6D].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.h = 0;
    solution.l = 0x42;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x6E].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    solution.l = 1;
    cpu.instruction_set_[0x6F].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov A", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 6;
  cpu.reg_.l = 7;

  cpu.memory_.flashReadOnlySegment("rom", {0x42});

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;
    solution.a = 2;
    cpu.instruction_set_[0x78].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ C")
  {
    solution.pc++;
    solution.a = 3;
    cpu.instruction_set_[0x79].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ D")
  {
    solution.pc++;
    solution.a = 4;

    cpu.instruction_set_[0x7A].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ E")
  {
    solution.pc++;
    solution.a = 5;
    cpu.instruction_set_[0x7B].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ H")
  {
    solution.pc++;
    solution.a = 6;
    cpu.instruction_set_[0x7C].exp();
    REQUIRE(cpu.reg_ == solution);
  }
  SECTION("reg_ L")
  {
    solution.pc++;
    solution.a = 7;
    cpu.instruction_set_[0x7D].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ M")
  {
    solution.pc++;
    solution.h = 0;
    solution.l = 0;
    solution.a = 0x42;

    cpu.reg_.h = 0;
    cpu.reg_.l = 0;
    cpu.instruction_set_[0x7E].exp();
    REQUIRE(cpu.reg_ == solution);
  }

  SECTION("reg_ A")
  {
    solution.pc++;
    cpu.instruction_set_[0x7F].exp();
    REQUIRE(cpu.reg_ == solution);
  }
}

TEST_CASE("Mov Mem", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.reg_.a = 1;
  cpu.reg_.b = 2;
  cpu.reg_.c = 3;
  cpu.reg_.d = 4;
  cpu.reg_.e = 5;
  cpu.reg_.h = 0x20;
  cpu.reg_.l = 0x00;

  auto solution = cpu.reg_;

  SECTION("reg_ B")
  {
    solution.pc++;

    cpu.instruction_set_[0x70].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.b);
  }

  SECTION("reg_ C")
  {
    solution.pc++;

    cpu.instruction_set_[0x71].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.c);
  }

  SECTION("reg_ D")
  {
    solution.pc++;

    cpu.instruction_set_[0x72].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.d);
  }

  SECTION("reg_ E")
  {
    solution.pc++;

    cpu.instruction_set_[0x73].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.e);
  }

  SECTION("reg_ H")
  {
    solution.pc++;

    cpu.instruction_set_[0x74].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.h);
  }

  SECTION("reg_ L")
  {
    solution.pc++;

    cpu.instruction_set_[0x75].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.l);
  }

  SECTION("reg_ A")
  {
    solution.pc++;

    cpu.instruction_set_[0x77].exp();
    REQUIRE(cpu.reg_ == solution);
    REQUIRE(cpu.memory_[0x2000] == cpu.reg_.a);
  }
}