#include <catch2/catch.hpp>
#include <iostream>
#include <vector>
#include "memory.hpp"
using namespace smart_memory;
TEST_CASE("Flash Segment", "[Type]")
{
  Memory<uint8_t> memory;
  memory.pushSegment({"rom", 0x20, MemoryAttribute::READ_ONLY});
  memory.pushSegment({"ram", 0x20, MemoryAttribute::READ_WRITE});

  SECTION("Flash")
  {
    REQUIRE(memory[0x00] == 0);
    memory.flashReadOnlySegment("rom", {0xDE, 0xAD, 0xBE, 0xEF});

    REQUIRE(memory[0x00] == 0xDE);
    REQUIRE(memory[0x01] == 0xAD);
    REQUIRE(memory[0x02] == 0xBE);
    REQUIRE(memory[0x03] == 0xEF);

    REQUIRE(memory[0x04] == 0x00);

    memory.flashReadOnlySegment("rom", {0xAA, 0x55});

    REQUIRE(memory[0x00] == 0xAA);
    REQUIRE(memory[0x01] == 0x55);
    REQUIRE(memory[0x02] == 0x00);
  }
}

TEST_CASE("Constructor", "[Type]")
{
  SECTION("Push")
  {
    Memory<uint8_t> memory;
    memory.pushSegment({"rom", 0x20, MemoryAttribute::READ_ONLY});
    memory.pushSegment({"ram", 0x20, MemoryAttribute::READ_WRITE});

    for (auto i = 0; i < 0x40; i++)
    {
      INFO("Failed index is " << i);
      REQUIRE(memory[i] == 0);
    }
  }

  SECTION("Push")
  {
    std::vector<MemorySegment> seg;
    seg.push_back({"rom", 0x20, MemoryAttribute::READ_ONLY});
    seg.push_back({"ram", 0x20, MemoryAttribute::READ_WRITE});

    Memory<uint8_t> memory{seg};
    for (auto i = 0; i < 0x40; i++)
    {
      INFO("Failed index is " << i);
      REQUIRE(memory[i] == 0);
    }
  }
}

TEST_CASE("Memory Operators", "[type]")
{
  Memory<uint8_t> memory;

  memory.pushSegment({"rom", 0x20, MemoryAttribute::READ_ONLY});
  memory.pushSegment({"ram", 0x20, MemoryAttribute::READ_WRITE});
  memory.pushSegment({"private", 0x20, MemoryAttribute::WRITE_ONLY});

  SECTION("Assignment")
  {
    REQUIRE(memory.readMemory(0x20) == 0);
    memory[0x20] = 0x42;
    REQUIRE(memory.readMemory(0x20) == 0x42);

    REQUIRE_THROWS_AS(memory[0x0] = 1, std::runtime_error);
    REQUIRE_THROWS_AS(memory[0x60] = 1, std::runtime_error);
    REQUIRE_THROWS_AS(memory[0x80] = 1, std::runtime_error);
  }
  SECTION("Operator Read")
  {
    REQUIRE(memory[0x00] == 0);
    memory[0x20] = 0x42;
    REQUIRE(memory[0x20] == 0x42);

    REQUIRE_THROWS_AS(memory[0x41] == 1, std::runtime_error);
    REQUIRE_THROWS_AS(memory[0x80] == 1, std::runtime_error);
  }
}