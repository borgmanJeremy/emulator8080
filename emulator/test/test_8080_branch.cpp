#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("RESTART", "[type")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.reg_.sp = 0x2300;
  cpu.reg_.pc = 0x00;

  SECTION("RST 0")
  {
    cpu.instruction_set_[0xC7].exp();
    REQUIRE(cpu.reg_.pc == 0x00);
  }
  SECTION("RST 1")
  {
    cpu.instruction_set_[0xCF].exp();
    REQUIRE(cpu.reg_.pc == 0x08);
  }

  SECTION("RST 2")
  {
    cpu.instruction_set_[0xD7].exp();
    REQUIRE(cpu.reg_.pc == 0x10);
  }

  SECTION("RST 3")
  {
    cpu.instruction_set_[0xDF].exp();
    REQUIRE(cpu.reg_.pc == 0x18);
  }

  SECTION("RST 4")
  {
    cpu.instruction_set_[0xE7].exp();
    REQUIRE(cpu.reg_.pc == 0x20);
  }

  SECTION("RST 5")
  {
    cpu.instruction_set_[0xEF].exp();
    REQUIRE(cpu.reg_.pc == 0x28);
  }

  SECTION("RST 6")
  {
    cpu.instruction_set_[0xF7].exp();
    REQUIRE(cpu.reg_.pc == 0x30);
  }

  SECTION("RST 7")
  {
    cpu.instruction_set_[0xFF].exp();
    REQUIRE(cpu.reg_.pc == 0x38);
  }

  REQUIRE(cpu.memory_[cpu.reg_.sp] == 0x01);
  REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0x00);
}

TEST_CASE("CALL ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.memory_.flashReadOnlySegment("rom",
                                   {0x00, 0x00, 0xCD, 0x07, 0x00, 0xC3, 0x00});
  cpu.reg_.sp = 0x2300;

  SECTION("Call / Ret")
  {
    cpu.reg_.pc = 2;
    cpu.instruction_set_[0xCD].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
    REQUIRE(cpu.memory_[cpu.reg_.sp] == 0x05);
    REQUIRE(cpu.memory_[cpu.reg_.sp + 1] == 0x00);

    cpu.instruction_set_[0xC9].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("CNZ - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.z = 0;
    cpu.instruction_set_[0xC4].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CNZ - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.z = 1;
    cpu.instruction_set_[0xC4].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CZ - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.z = 1;
    cpu.instruction_set_[0xCC].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CZ - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.z = 0;
    cpu.instruction_set_[0xCC].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CNC - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.cy = 0;
    cpu.instruction_set_[0xD4].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CNC - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xD4].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CC - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xDC].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CC - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.cy = 0;
    cpu.instruction_set_[0xDC].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CPO - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.p = 0;
    cpu.instruction_set_[0xE4].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CPO - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.p = 1;
    cpu.instruction_set_[0xE4].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CPE - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.p = 1;
    cpu.instruction_set_[0xEC].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CPE - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.p = 0;
    cpu.instruction_set_[0xEC].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CP - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.s = 0;
    cpu.instruction_set_[0xF4].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CP - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.s = 1;
    cpu.instruction_set_[0xF4].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }

  SECTION("CM - Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.s = 1;
    cpu.instruction_set_[0xFC].exp();
    REQUIRE(cpu.reg_.pc == 0x0007);
  }

  SECTION("CM - Dont Call")
  {
    cpu.reg_.pc = 2;
    cpu.flags_.s = 0;
    cpu.instruction_set_[0xFC].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
  }
}

TEST_CASE("RET ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.memory_.flashReadOnlySegment("rom",
                                   {0x00, 0x00, 0xCD, 0x07, 0x00, 0xC3, 0x00});

  cpu.reg_.pc = 7;
  cpu.reg_.sp = 0x2300 - 2;
  cpu.memory_[0x2300 - 1] = 0;
  cpu.memory_[0x2300 - 2] = 0x05;

  SECTION("Ret")
  {
    cpu.instruction_set_[0xC9].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RNZ -Ret")
  {
    cpu.flags_.z = 0;
    cpu.instruction_set_[0xC0].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RNZ - Don't Ret")
  {
    cpu.flags_.z = 1;
    cpu.instruction_set_[0xC0].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }

  SECTION("RZ -Ret")
  {
    cpu.flags_.z = 1;
    cpu.instruction_set_[0xC8].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RZ - Don't Ret")
  {
    cpu.flags_.z = 0;
    cpu.instruction_set_[0xC8].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }
  SECTION("RNC -Ret")
  {
    cpu.flags_.cy = 0;
    cpu.instruction_set_[0xD0].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RNC - Don't Ret")
  {
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xD0].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }

  SECTION("RC -Ret")
  {
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xD8].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RC - Don't Ret")
  {
    cpu.flags_.cy = 0;
    cpu.instruction_set_[0xD8].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }

  SECTION("RPO -Ret")
  {
    cpu.flags_.p = 0;
    cpu.instruction_set_[0xE0].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RPO - Don't Ret")
  {
    cpu.flags_.p = 1;
    cpu.instruction_set_[0xE0].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }

  SECTION("RPE -Ret")
  {
    cpu.flags_.p = 1;
    cpu.instruction_set_[0xE8].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RPE - Don't Ret")
  {
    cpu.flags_.p = 0;
    cpu.instruction_set_[0xE8].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }

  SECTION("RP -Ret")
  {
    cpu.flags_.s = 0;
    cpu.instruction_set_[0xF0].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RP - Don't Ret")
  {
    cpu.flags_.s = 1;
    cpu.instruction_set_[0xF0].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }

  SECTION("RM -Ret")
  {
    cpu.flags_.s = 1;
    cpu.instruction_set_[0xF8].exp();
    REQUIRE(cpu.reg_.pc == 0x0005);
    REQUIRE(cpu.reg_.sp == 0x2300);
  }

  SECTION("RM - Don't Ret")
  {
    cpu.flags_.s = 0;
    cpu.instruction_set_[0xF8].exp();
    REQUIRE(cpu.reg_.pc == 0x0008);
    REQUIRE(cpu.reg_.sp == 0x2300 - 2);
  }
}

TEST_CASE("Branch ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.memory_.flashReadOnlySegment("rom", {0x00, 0x12, 0x34});
  cpu.reg_.pc = 0;

  SECTION("PCHL")
  {
    cpu.reg_.h = 0x23;
    cpu.reg_.l = 0x00;
    cpu.instruction_set_[0xE9].exp();
    REQUIRE(cpu.reg_.pc == 0x2300);
  }

  SECTION("Jump")
  {
    cpu.instruction_set_[0xC3].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);
  }

  SECTION("JNZ")
  {
    cpu.flags_.z = 1;
    cpu.instruction_set_[0xC2].exp();
    REQUIRE(cpu.reg_.pc == 0x03);

    cpu.reg_.pc = 0;
    cpu.flags_.z = 0;
    cpu.instruction_set_[0xC2].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);
  }

  SECTION("JZ")
  {
    cpu.flags_.z = 0;
    cpu.instruction_set_[0xCA].exp();
    REQUIRE(cpu.reg_.pc == 0x03);

    cpu.reg_.pc = 0;
    cpu.flags_.z = 1;
    cpu.instruction_set_[0xCA].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);
  }

  SECTION("JNC")
  {
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xD2].exp();
    REQUIRE(cpu.reg_.pc == 0x03);

    cpu.reg_.pc = 0;
    cpu.flags_.cy = 0;
    cpu.instruction_set_[0xD2].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);
  }

  SECTION("JC")
  {
    cpu.flags_.cy = 0;
    cpu.instruction_set_[0xDA].exp();
    REQUIRE(cpu.reg_.pc == 0x03);

    cpu.reg_.pc = 0;
    cpu.flags_.cy = 1;
    cpu.instruction_set_[0xDA].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);
  }

  SECTION("JPO")
  {
    cpu.flags_.p = 0;
    cpu.instruction_set_[0xE2].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);

    cpu.reg_.pc = 0;
    cpu.flags_.p = 1;
    cpu.instruction_set_[0xE2].exp();
    REQUIRE(cpu.reg_.pc == 0x03);
  }

  SECTION("JPE")
  {
    cpu.flags_.p = 1;
    cpu.instruction_set_[0xEA].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);

    cpu.reg_.pc = 0;
    cpu.flags_.p = 0;
    cpu.instruction_set_[0xEA].exp();
    REQUIRE(cpu.reg_.pc == 0x03);
  }

  SECTION("JP")
  {
    cpu.flags_.s = 0;
    cpu.instruction_set_[0xF2].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);

    cpu.reg_.pc = 0;
    cpu.flags_.s = 1;
    cpu.instruction_set_[0xF2].exp();
    REQUIRE(cpu.reg_.pc == 0x03);
  }

  SECTION("JM")
  {
    cpu.flags_.s = 1;
    cpu.instruction_set_[0xFA].exp();
    REQUIRE(cpu.reg_.pc == 0x3412);

    cpu.reg_.pc = 0;
    cpu.flags_.s = 0;
    cpu.instruction_set_[0xFA].exp();
    REQUIRE(cpu.reg_.pc == 0x03);
  }
}
