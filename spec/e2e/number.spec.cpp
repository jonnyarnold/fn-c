#include "spec/spec.h"

TEST_CASE("Integer set/get") {
  REQUIRE(resultOf("x = 1; x") == "1");
}

TEST_CASE("Double set/get") {
  REQUIRE(resultOf("x = 1.23; x") == "1.230000");
}

// TODO: Mathematical precedence
//
// TEST_CASE("Mathemtical operators are in precedence") {
//   REQUIRE(resultOf("2 + 2 * 2") == "6");
// }

// TODO: Negation operator
//
// TEST_CASE("Negative number set/get") {
//   REQUIRE(resultOf("x = -1; x") == "-1");
// }
