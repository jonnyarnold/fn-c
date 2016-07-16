#include "spec/spec.h"

TEST_CASE("Integer set/get") {
  REQUIRE(resultOf("x = 1; x") == "1");
}

TEST_CASE("Double set/get") {
  REQUIRE(resultOf("x = 1.23; x") == "1.230000");
}
