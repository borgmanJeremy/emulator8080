#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("STC")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x00;

  cpu.flags_.cy = 0;
  cpu.instruction_set_[0x37].exp();
  REQUIRE(cpu.reg_.pc == 1);
  REQUIRE(cpu.flags_.cy == 1);
}

TEST_CASE("Rotate", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x00;

  SECTION("RLC")
  {
    cpu.flags_.cy = 0;
    cpu.reg_.a = 0xF2;
    cpu.instruction_set_[0x07].exp();

    REQUIRE(cpu.reg_.a == 0xE5);
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("RRC")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xF2;
    cpu.instruction_set_[0x0F].exp();

    REQUIRE(cpu.reg_.a == 0x79);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("RAL")
  {
    cpu.flags_.cy = 0;
    cpu.reg_.a = 0xB5;
    cpu.instruction_set_[0x17].exp();

    REQUIRE(cpu.reg_.a == 0x6A);
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("RAR")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0x6A;
    cpu.instruction_set_[0x1F].exp();

    REQUIRE(cpu.reg_.a == 0xB5);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
}

TEST_CASE("INR", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x00;

  SECTION("reg B")
  {
    cpu.reg_.b = 0x01;

    cpu.instruction_set_[0x04].exp();
    REQUIRE(cpu.reg_.b == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg C")
  {
    cpu.reg_.c = 0x01;

    cpu.instruction_set_[0x0C].exp();
    REQUIRE(cpu.reg_.c == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
  SECTION("reg D")
  {
    cpu.reg_.d = 0x01;

    cpu.instruction_set_[0x14].exp();
    REQUIRE(cpu.reg_.d == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg E")
  {
    cpu.reg_.e = 0x01;

    cpu.instruction_set_[0x1C].exp();
    REQUIRE(cpu.reg_.e == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
  SECTION("reg H")
  {
    cpu.reg_.h = 0x01;

    cpu.instruction_set_[0x24].exp();
    REQUIRE(cpu.reg_.h == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
  SECTION("reg L")
  {
    cpu.reg_.l = 0x01;

    cpu.instruction_set_[0x2C].exp();
    REQUIRE(cpu.reg_.l == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg M")
  {
    cpu.reg_.h = 0x20;
    cpu.reg_.l = 0x00;
    cpu.memory_[0x2000] = 1;

    cpu.instruction_set_[0x34].exp();
    REQUIRE(cpu.memory_[0x2000] == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg A")
  {
    cpu.reg_.a = 0x01;

    cpu.instruction_set_[0x3C].exp();
    REQUIRE(cpu.reg_.a == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}

TEST_CASE("LXI ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x00;

  SECTION("reg BC")
  {
    cpu.reg_.b = 0x01;
    cpu.reg_.c = 0xFF;

    cpu.instruction_set_[0x03].exp();
    REQUIRE(cpu.reg_.b == 0x02);
    REQUIRE(cpu.reg_.c == 0x00);
    REQUIRE(cpu.reg_.pc == 0x01);

    cpu.reg_.b = 0xFF;
    cpu.reg_.c = 0xFF;
    cpu.instruction_set_[0x03].exp();
    REQUIRE(cpu.reg_.b == 0x00);
    REQUIRE(cpu.reg_.c == 0x00);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
  SECTION("reg DE")
  {
    cpu.reg_.d = 0x01;
    cpu.reg_.e = 0xFF;

    cpu.instruction_set_[0x13].exp();
    REQUIRE(cpu.reg_.d == 0x02);
    REQUIRE(cpu.reg_.e == 0x00);
    REQUIRE(cpu.reg_.pc == 0x01);

    cpu.reg_.d = 0xFF;
    cpu.reg_.e = 0xFF;
    cpu.instruction_set_[0x13].exp();
    REQUIRE(cpu.reg_.d == 0x00);
    REQUIRE(cpu.reg_.e == 0x00);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("reg HL")
  {
    cpu.reg_.h = 0x01;
    cpu.reg_.l = 0xFF;

    cpu.instruction_set_[0x23].exp();
    REQUIRE(cpu.reg_.h == 0x02);
    REQUIRE(cpu.reg_.l == 0x00);
    REQUIRE(cpu.reg_.pc == 0x01);

    cpu.reg_.h = 0xFF;
    cpu.reg_.l = 0xFF;
    cpu.instruction_set_[0x23].exp();
    REQUIRE(cpu.reg_.h == 0x00);
    REQUIRE(cpu.reg_.l == 0x00);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("reg SP")
  {
    cpu.reg_.sp = 0x01;

    cpu.instruction_set_[0x33].exp();
    REQUIRE(cpu.reg_.sp == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);

    cpu.reg_.sp = 0xFFFF;
    cpu.instruction_set_[0x33].exp();
    REQUIRE(cpu.reg_.sp == 0x00);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
}

TEST_CASE("DCR", "[type")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x00;

  SECTION("reg B")
  {
    cpu.reg_.b = 0x03;

    cpu.instruction_set_[0x05].exp();
    REQUIRE(cpu.reg_.b == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg C")
  {
    cpu.reg_.c = 0x03;

    cpu.instruction_set_[0x0D].exp();
    REQUIRE(cpu.reg_.c == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
  SECTION("reg D")
  {
    cpu.reg_.d = 0x03;

    cpu.instruction_set_[0x15].exp();
    REQUIRE(cpu.reg_.d == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg E")
  {
    cpu.reg_.e = 0x03;

    cpu.instruction_set_[0x1D].exp();
    REQUIRE(cpu.reg_.e == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
  SECTION("reg H")
  {
    cpu.reg_.h = 0x03;

    cpu.instruction_set_[0x25].exp();
    REQUIRE(cpu.reg_.h == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
  SECTION("reg L")
  {
    cpu.reg_.l = 0x03;

    cpu.instruction_set_[0x2D].exp();
    REQUIRE(cpu.reg_.l == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg M")
  {
    cpu.reg_.h = 0x20;
    cpu.reg_.l = 0x00;
    cpu.memory_[0x2000] = 3;

    cpu.instruction_set_[0x35].exp();
    REQUIRE(cpu.memory_[0x2000] == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("reg A")
  {
    cpu.reg_.a = 0x03;

    cpu.instruction_set_[0x3D].exp();
    REQUIRE(cpu.reg_.a == 0x02);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}