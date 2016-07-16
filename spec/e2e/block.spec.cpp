#include "spec/spec.h"

TEST_CASE("Block set/get") {
  REQUIRE(resultOf("x = {}; x") == "{}");
  REQUIRE(resultOf("x = { a = 1 }; x.a") == "1");
}

TEST_CASE("Assignment outside of block") {
  REQUIRE(resultOf("x = {}; x.y = 1; x.y") == "1");
}
