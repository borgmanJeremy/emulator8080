#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("CMA")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;

  cpu.reg_.a = 0x55;
  cpu.instruction_set_[0x2F].exp();
  REQUIRE(cpu.reg_.pc == 1);
  REQUIRE(cpu.reg_.a == 0xAA);
}

TEST_CASE("CMC")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;

  cpu.flags_.cy = 0x1;
  cpu.instruction_set_[0x3F].exp();
  REQUIRE(cpu.reg_.pc == 1);
  REQUIRE(cpu.flags_.cy == 0x00);

  cpu.flags_.cy = 0x00;
  cpu.instruction_set_[0x3F].exp();
  REQUIRE(cpu.reg_.pc == 2);
  REQUIRE(cpu.flags_.cy == 0x01);
}

TEST_CASE("Logical AND ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.memory_.flashReadOnlySegment("rom", {0xAA});

  SECTION("reg_ B")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0;
    cpu.instruction_set_[0xA0].exp();
    REQUIRE(cpu.reg_.a == 0x00);
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0xFF;
    cpu.instruction_set_[0xA0].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0xAA;
    cpu.instruction_set_[0xA0].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 3);
  }

  SECTION("reg_ C")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.c = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xA1].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ D")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.d = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xA2].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ E")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.e = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xA3].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ H")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xA4].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ L")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.l = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xA5].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("Data")
  {
    cpu.reg_.a = 0xFF;

    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xAA});

    cpu.instruction_set_[0xE6].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 2);
  }
  SECTION("reg_ M")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.l = 0x00;
    cpu.reg_.h = 0x00;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0xA6].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ A")
  {
    cpu.reg_.a = 0xAA;
    cpu.flags_.cy = 1;

    cpu.instruction_set_[0xA7].exp();
    REQUIRE(cpu.reg_.a == 0xAA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
}

TEST_CASE("Logical XOR ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.memory_.flashReadOnlySegment("rom", {0xAA});

  SECTION("reg_ B")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0;
    cpu.instruction_set_[0xA8].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0xFF;
    cpu.instruction_set_[0xA8].exp();
    REQUIRE(cpu.reg_.a == 0x00);
    REQUIRE(cpu.flags_.cy == 0);

    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0xAA;
    cpu.instruction_set_[0xA8].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 3);
  }

  SECTION("reg_ C")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.c = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xA9].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ D")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.d = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xAA].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ E")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.e = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xAB].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ H")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.h = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xAC].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ L")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.l = 0xAA;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xAD].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("Data")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xAA});
    cpu.reg_.a = 0xFF;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xEE].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ M")
  {
    cpu.reg_.a = 0xFF;
    cpu.reg_.l = 0x00;
    cpu.reg_.h = 0x00;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xAE].exp();
    REQUIRE(cpu.reg_.a == 0x55);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ A")
  {
    cpu.reg_.a = 0xAF;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xAF].exp();
    REQUIRE(cpu.reg_.a == 0x00);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
}
TEST_CASE("Logical OR ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.memory_.flashReadOnlySegment("rom", {0xAA});

  SECTION("reg_ B")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.b = 0xAA;
    cpu.instruction_set_[0xB0].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ C")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.c = 0xAA;
    cpu.instruction_set_[0xB1].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ D")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.d = 0xAA;
    cpu.instruction_set_[0xB2].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ E")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.e = 0xAA;
    cpu.instruction_set_[0xB3].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ H")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.h = 0xAA;
    cpu.instruction_set_[0xB4].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ L")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.l = 0xAA;
    cpu.instruction_set_[0xB5].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("Data")
  {
    cpu.memory_.flashReadOnlySegment("rom", {0x00, 0xAA});
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.instruction_set_[0xF6].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 2);
  }

  SECTION("reg_ M")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.reg_.l = 0x00;
    cpu.reg_.h = 0x00;
    cpu.instruction_set_[0xB6].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ A")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFA;
    cpu.instruction_set_[0xB7].exp();
    REQUIRE(cpu.reg_.a == 0xFA);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
}

TEST_CASE("Logical CMP ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.pc = 0;
  cpu.memory_.flashReadOnlySegment("rom", {0x05});

  SECTION("reg_ B")
  {
    cpu.flags_.cy = 1;
    cpu.reg_.a = 0xFF;
    cpu.reg_.b = 0xFF;
    cpu.instruction_set_[0xB8].exp();
    REQUIRE(cpu.reg_.a == 0xFF);
    REQUIRE(cpu.reg_.b == 0xFF);
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.flags_.z == 1);

    cpu.flags_.cy = 1;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x0A;
    cpu.reg_.b = 0x05;
    cpu.instruction_set_[0xB8].exp();
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.flags_.z == 0);

    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.b = 0x05;
    cpu.instruction_set_[0xB8].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 3);
  }

  SECTION("reg_ C")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.c = 0x05;
    cpu.instruction_set_[0xB9].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ D")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.d = 0x05;
    cpu.instruction_set_[0xBA].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ E")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.e = 0x05;
    cpu.instruction_set_[0xBB].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ H")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.h = 0x05;
    cpu.instruction_set_[0xBC].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ L")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.l = 0x05;
    cpu.instruction_set_[0xBD].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }

  SECTION("reg_ M")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.reg_.l = 0x00;
    cpu.reg_.h = 0x00;
    cpu.instruction_set_[0xBE].exp();
    REQUIRE(cpu.flags_.cy == 1);
    REQUIRE(cpu.flags_.z == 0);
    REQUIRE(cpu.reg_.pc == 1);
  }
  SECTION("reg_ A")
  {
    cpu.flags_.cy = 0;
    cpu.flags_.z = 1;
    cpu.reg_.a = 0x02;
    cpu.instruction_set_[0xBF].exp();
    REQUIRE(cpu.flags_.cy == 0);
    REQUIRE(cpu.flags_.z == 1);
    REQUIRE(cpu.reg_.pc == 1);
  }
}