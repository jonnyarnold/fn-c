#include "spec/spec.h"

TEST_CASE("Positive integer set/get") {
  REQUIRE(resultOf("x = 1; x")->asNumber() == fn::Number(0, 1));
}

TEST_CASE("Negative integer set/get") {
  REQUIRE(resultOf("x = -1; x")->asNumber() == fn::Number(0, -1));
}

TEST_CASE("Positive decimal set/get") {
  REQUIRE(resultOf("x = 1.23; x")->asNumber() == fn::Number(-2, 123));
}

TEST_CASE("Negative decimal set/get") {
  REQUIRE(resultOf("x = -1.23; x")->asNumber() == fn::Number(-2, -123));
}

TEST_CASE("Addition (positive integers)") {
  REQUIRE(resultOf("x = 2 + 0; x")->asNumber() == fn::Number(0, 2));
  REQUIRE(resultOf("x = 2 + 2; x")->asNumber() == fn::Number(0, 4));
}

TEST_CASE("Addition (decimals and integers)") {
  REQUIRE(resultOf("x = 0.2 + 2; x")->asNumber() == fn::Number(-1, 22));
  REQUIRE(resultOf("x = 0.3 + 0.3; x")->asNumber() == fn::Number(-1, 6));
}

TEST_CASE("Addition (positive and negative)") {
  REQUIRE(resultOf("x = 2 + (-2); x")->asNumber() == fn::Number(0, 0));
  REQUIRE(resultOf("x = (-2) + 4; x")->asNumber() == fn::Number(0, 2));
  REQUIRE(resultOf("x = 0.3 + (-0.6); x")->asNumber() == fn::Number(-1, -3));
}

TEST_CASE("Subtraction (positive integers)") {
  REQUIRE(resultOf("x = 2 - 0; x")->asNumber() == fn::Number(0, 2));
  REQUIRE(resultOf("x = 4 - 2; x")->asNumber() == fn::Number(0, 2));
  REQUIRE(resultOf("x = 2 - 4; x")->asNumber() == fn::Number(0, -2));
}

TEST_CASE("Subtraction (decimals and integers)") {
  REQUIRE(resultOf("x = 2 - 0.2; x")->asNumber() == fn::Number(-1, 18));
  REQUIRE(resultOf("x = 0.9 - 0.3; x")->asNumber() == fn::Number(-1, 6));
}

TEST_CASE("Subtraction (positive and negative)") {
  REQUIRE(resultOf("x = 2 - (-2); x")->asNumber() == fn::Number(0, 4));
  REQUIRE(resultOf("x = (-2) - 4; x")->asNumber() == fn::Number(0, -6));
  REQUIRE(resultOf("x = (-0.3) - (-0.6); x")->asNumber() == fn::Number(-1, 3));
}

TEST_CASE("Multiplication (positive integers)") {
  REQUIRE(resultOf("x = 2 * 0; x")->asNumber() == fn::Number(0, 0));
  REQUIRE(resultOf("x = 2 * 2; x")->asNumber() == fn::Number(0, 4));
  REQUIRE(resultOf("x = 2 * 20; x")->asNumber() == fn::Number(0, 40));
}

TEST_CASE("Multiplication (decimals and integers)") {
  REQUIRE(resultOf("x = 0.2 * 2; x")->asNumber() == fn::Number(-1, 4));
  REQUIRE(resultOf("x = 0.5 * 0.5; x")->asNumber() == fn::Number(-2, 25));
}

TEST_CASE("Multiplication (positive and negative)") {
  REQUIRE(resultOf("x = 2 * (-2); x")->asNumber() == fn::Number(0, -4));
  REQUIRE(resultOf("x = (-2) * (-2); x")->asNumber() == fn::Number(0, 4));
}

TEST_CASE("Division (positive integers)") {
  // TODO: Divide by 0?
  REQUIRE(resultOf("x = 2 / 2; x")->asNumber() == fn::Number(0, 1));
  REQUIRE(resultOf("x = 4 / 2; x")->asNumber() == fn::Number(0, 2));
  REQUIRE(resultOf("x = 20 / 4; x")->asNumber() == fn::Number(0, 5));
  // FIXME
  // REQUIRE(resultOf("x = 10 / 4; x")->asNumber() == fn::Number(-1, 25));
}

TEST_CASE("Division (decimals and integers)") {
  REQUIRE(resultOf("x = 0.2 / 2; x")->asNumber() == fn::Number(-1, 1));
  REQUIRE(resultOf("x = 10 / 0.5; x")->asNumber() == fn::Number(0, 20));
}

TEST_CASE("Division (positive and negative)") {
  REQUIRE(resultOf("x = 2 / (-2); x")->asNumber() == fn::Number(0, -1));
  REQUIRE(resultOf("x = (-2) / (-2); x")->asNumber() == fn::Number(0, 1));
}

// TODO: Mathematical precedence
//
// TEST_CASE("Mathemtical operators are in precedence") {
//   REQUIRE(resultOf("2 + 2 * 2") == "6");
// }

// TEST_CASE(".moreThan()") {
//   SECTION("is false when < y") {
//     REQUIRE(resultOf("x = 1; x.moreThan(1.1)") == "false");
//   }

//   SECTION("is true when > y") {
//     REQUIRE(resultOf("x = 1; x.moreThan(0.9)") == "true");
//   }

//   SECTION("is false when == y") {
//     REQUIRE(resultOf("x = 1; x.moreThan(1)") == "false");
//   }
// }


// TEST_CASE(".lessThan()") {
//   SECTION("is false when > y") {
//     REQUIRE(resultOf("x = 1; x.lessThan(0.9)") == "false");
//   }

//   SECTION("is true when < y") {
//     REQUIRE(resultOf("x = 1; x.lessThan(1.1)") == "true");
//   }

//   SECTION("is false when == y") {
//     REQUIRE(resultOf("x = 1; x.lessThan(1)") == "false");
//   }
// }
