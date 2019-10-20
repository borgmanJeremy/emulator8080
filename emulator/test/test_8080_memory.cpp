#include <catch2/catch.hpp>
#include "cpu_8080.hpp"

TEST_CASE("Test HL ", "[type]")
{
  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"ram", 0x20, smart_memory::MemoryAttribute::READ_WRITE});

  SECTION("Load from HL")
  {
    cpu.reg_.h = 0x00;
    cpu.reg_.l = 0x01;
    REQUIRE(cpu.getAddressFromHL() == 1);

    cpu.reg_.h = 0x10;
    cpu.reg_.l = 0x00;
    REQUIRE(cpu.getAddressFromHL() == 0x1000);
  }
}