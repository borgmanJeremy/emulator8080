#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("STA")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x0;
  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0x00, 0x20});

  cpu.reg_.a = 0xDE;
  cpu.instruction_set_[0x32].exp();

  REQUIRE(cpu.memory_[0x2000] == 0xDE);
  REQUIRE(cpu.reg_.pc == 0x03);
}

TEST_CASE("LDA")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x0;
  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0x00, 0x20});

  cpu.memory_[0x2000] = 0xDE;
  cpu.instruction_set_[0x3A].exp();

  REQUIRE(cpu.reg_.a == 0xDE);
  REQUIRE(cpu.reg_.pc == 0x03);
}

TEST_CASE("SHLD")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x0;
  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0x00, 0x20});

  cpu.reg_.l = 0xDE;
  cpu.reg_.h = 0xAD;

  cpu.instruction_set_[0x22].exp();

  REQUIRE(cpu.memory_[0x2000] == 0xDE);
  REQUIRE(cpu.memory_[0x2001] == 0xAD);
  REQUIRE(cpu.reg_.pc == 0x03);
}

TEST_CASE("LHLD")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x0;
  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0x00, 0x20});

  cpu.memory_[0x2000] = 0xDE;
  cpu.memory_[0x2001] = 0xAD;

  cpu.instruction_set_[0x2A].exp();

  REQUIRE(cpu.reg_.l == 0xDE);
  REQUIRE(cpu.reg_.h == 0xAD);
  REQUIRE(cpu.reg_.pc == 0x03);
}

TEST_CASE("MVI", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0x00;
  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xDE, 0xAD});

  SECTION("B")
  {
    cpu.instruction_set_[0x06].exp();
    REQUIRE(cpu.reg_.b == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("C")
  {
    cpu.instruction_set_[0x0E].exp();
    REQUIRE(cpu.reg_.c == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
  SECTION("D")
  {
    cpu.instruction_set_[0x16].exp();
    REQUIRE(cpu.reg_.d == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
  SECTION("E")
  {
    cpu.instruction_set_[0x1E].exp();
    REQUIRE(cpu.reg_.e == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
  SECTION("H")
  {
    cpu.instruction_set_[0x26].exp();
    REQUIRE(cpu.reg_.h == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
  SECTION("L")
  {
    cpu.instruction_set_[0x2E].exp();
    REQUIRE(cpu.reg_.l == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("Mem")
  {
    cpu.reg_.h = 0x20;
    cpu.reg_.l = 0x00;
    cpu.memory_[0x2000] = 0xAC;

    cpu.instruction_set_[0x36].exp();
    REQUIRE(cpu.memory_[0x2000] == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }

  SECTION("A")
  {
    cpu.instruction_set_[0x3E].exp();
    REQUIRE(cpu.reg_.a == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x02);
  }
}

TEST_CASE("LDAX", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.memory_[0x2000] = 0xDE;
  cpu.memory_[0x2001] = 0xAD;

  SECTION("BC")
  {
    cpu.reg_.b = 0x20;
    cpu.reg_.c - 0x00;
    cpu.instruction_set_[0x0A].exp();

    REQUIRE(cpu.reg_.a == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("DE")
  {
    cpu.reg_.d = 0x20;
    cpu.reg_.e - 0x00;
    cpu.instruction_set_[0x1A].exp();

    REQUIRE(cpu.reg_.a == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}

TEST_CASE("STAX", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.memory_[0x2000] = 0x00;
  cpu.memory_[0x2001] = 0x00;
  cpu.reg_.a = 0xDE;

  SECTION("BC")
  {
    cpu.reg_.b = 0x20;
    cpu.reg_.c - 0x00;
    cpu.instruction_set_[0x02].exp();

    REQUIRE(cpu.memory_[0x2000] == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x01);
  }

  SECTION("DE")
  {
    cpu.reg_.d = 0x20;
    cpu.reg_.e - 0x00;
    cpu.instruction_set_[0x12].exp();

    REQUIRE(cpu.memory_[0x2000] == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x01);
  }
}

TEST_CASE("LXI", "[reg]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xDE, 0xAD});
  cpu.reg_.pc = 0;

  SECTION("B")
  {
    cpu.instruction_set_[0x01].exp();
    REQUIRE(cpu.reg_.b == 0xAD);
    REQUIRE(cpu.reg_.c == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x03);
  }
  SECTION("D")
  {
    cpu.instruction_set_[0x11].exp();
    REQUIRE(cpu.reg_.d == 0xAD);
    REQUIRE(cpu.reg_.e == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x03);
  }

  SECTION("H")
  {
    cpu.instruction_set_[0x21].exp();
    REQUIRE(cpu.reg_.h == 0xAD);
    REQUIRE(cpu.reg_.l == 0xDE);
    REQUIRE(cpu.reg_.pc == 0x03);
  }

  SECTION("SP")
  {
    cpu.instruction_set_[0x31].exp();
    REQUIRE(cpu.reg_.sp == 0xADDE);
    REQUIRE(cpu.reg_.pc == 0x03);
  }
}