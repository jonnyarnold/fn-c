#include "spec/spec.h"

TEST_CASE("eq") {

//   SECTION("blocks are value-equal") {
//     REQUIRE(resultOf("{} eq {}") == "true");
//     REQUIRE(resultOf("{ x = 1 } eq { x = 1 }") == "true");
//     REQUIRE(resultOf("{ x = 1 } eq { x = 2 }") == "false");
//   }

  SECTION("bools are value-equal") {
    REQUIRE(resultOf("true eq true")->asBool() == true);
    REQUIRE(resultOf("true eq false")->asBool() == false);
  }

//   SECTION("defs are reference-equal") {
//     REQUIRE(resultOf(R"(
//       x = fn(a) { a + 1 }
//       y = fn(a) { a + 1 }
//       x eq y
//     )") == "false");

//     REQUIRE(resultOf(R"(
//       x = fn(a) { a + 1 }
//       y = x
//       x eq y
//     )") == "true");
//   }

//   SECTION("lists are value-equal") {
//     REQUIRE(resultOf("List(1) eq List(1)") == "true");
//     REQUIRE(resultOf("List(1) eq List(2)") == "false");
//   }

  SECTION("numbers are value-equal") {
    REQUIRE(resultOf("0 eq 0")->asBool() == true);
    REQUIRE(resultOf("0 eq 1")->asBool() == false);

    REQUIRE(resultOf("1 eq 1.0")->asBool() == true);
    REQUIRE(resultOf("1 eq 1.1")->asBool() == false);
  }

//   SECTION("strings are value-equal") {
//     REQUIRE(resultOf("\"foo\" eq \"foo\"") == "true");
//     REQUIRE(resultOf("\"foo\" eq \"bar\"") == "false");
//   }

}
