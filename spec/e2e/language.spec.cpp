#include "spec/spec.h"

// TEST_CASE("Empty programs compile") {
//   REQUIRE(resultOf("") == "{}");
// }

// TEST_CASE("Bad programs do not compile") {
//   REQUIRE(failure("({)}"));
// }

TEST_CASE("Semicolons") {
  REQUIRE(resultOf("x = true; x")->asBool() == true);
}

TEST_CASE("Brackets") {
  REQUIRE(resultOf("2 * (1 + 2)")->asNumber() == fn::Number(0, 6));
}

// TEST_CASE("Variable redefinition not allowed") {
//   REQUIRE(failure("a = 1; a = 2"));
// }

TEST_CASE("when") {
  SECTION("runs code on true") {
    REQUIRE(resultOf(R"(
      when {
        true { false }
      }
    )")->asBool() == false);
  }

  // TODO: Fix empty when case.
  //
  // SECTION("can be empty") {
  //   REQUIRE(resultOf("when {}") == "false");
  // }

  SECTION("runs the first true condition only") {
    REQUIRE(resultOf(R"(
      when {
        false { 1 }
        true { 2 }
        false { 3 }
      }
    )")->asNumber() == fn::Number(0, 2));
  }

  SECTION("returns false if no conditions match") {
    REQUIRE(resultOf(R"(
      when {
        false { true }
      }
    )")->asBool() == false);
  }

  SECTION("keeps value references") {
    REQUIRE(resultOf(R"(
      x = 2
      when {
        x eq 1 { 1 }
        x eq 2 { 2 }
      }
    )")->asNumber() == fn::Number(0, 2));
  }
}
