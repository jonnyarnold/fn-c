#include "spec/spec.h"

TEST_CASE("Empty programs compile") {
  REQUIRE(resultOf("") == "{}");
}

TEST_CASE("Semicolons") {
  REQUIRE(resultOf("x = 1; x") == "1");
}

TEST_CASE("Brackets") {
  REQUIRE(resultOf("2 * (1 + 2)") == "6");
}

// TODO: Mathematical operator precedence
