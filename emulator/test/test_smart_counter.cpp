#include <catch2/catch.hpp>
#include <iostream>
#include <vector>
#include "smart_counter.hpp"
TEST_CASE("Smart Counter")
{
  SmartCounter<unsigned int> count(100);
  count += 1;
  REQUIRE(count.value_ == 1);

  count = 0;
  REQUIRE(count.value_ == 0);

  count = 99;
  REQUIRE_THROWS_AS(count += 1, std::runtime_error);
  REQUIRE_THROWS_AS(count = 100, std::runtime_error);
}