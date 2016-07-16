#include "spec/spec.h"

TEST_CASE("Empty programs compile") {
  REQUIRE(resultOf("") == "{}");
}

// TODO: failure() doesn't work yet,
// because the compiler has no error handling!!!
//
// TEST_CASE("Bad programs do not compile") {
//   REQUIRE(failure("({)}"));
// }

TEST_CASE("Semicolons") {
  REQUIRE(resultOf("x = 1; x") == "1");
}

TEST_CASE("Brackets") {
  REQUIRE(resultOf("2 * (1 + 2)") == "6");
}

// TODO: failure() doesn't work yet,
// because the compiler has no error handling!!!
//
// TEST_CASE("Variable redefinition not allowed") {
//   REQUIRE(failure("a = 1; a = 2"));
// }

TEST_CASE("when") {
  SECTION("runs code on true") {
    REQUIRE(resultOf(R"(
      when {
        true { false }
      }
    )") == "false");
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
    )") == "2");
  }

  SECTION("returns false if no conditions match") {
    REQUIRE(resultOf(R"(
      when {
        false { true }
      }
    )") == "false");
  }
}
