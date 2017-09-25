#include "spec/spec.h"

TEST_CASE("Fn set/call") {
  REQUIRE(resultOf("x = fn (a) { a + 1 }; x(1)")->asNumber() == fn::Number(0, 2));
}

// TEST_CASE("Empty Fn") {
//   REQUIRE(resultOf("x = fn () {}; x()") == "{}");
// }

// TODO: Invalid number of arguments
//
// TEST_CASE("Error on invalid argument length") {
//   REQUIRE(failure("x = fn () {}; x(1)"));
// }

// TEST_CASE("Fn set/call in block") {
//   REQUIRE(resultOf(R"(
//     x = {
//       y = fn (a) { a + 1 }
//     }

//     x.y(1)
//   )")->asNumber() == fn::Number(0, 2));
// }

// TEST_CASE("Fn call within block") {
//   REQUIRE(resultOf(R"(
//     x = fn(a) {
//       foo = fn(b) { a + b }
//     }

//     (x(1)).foo(1)
//   )")->asNumber() == fn::Number(0, 2));
// }

// There was a time when we weren't passing params
// in the right order, every -other- time. Weird, huh?
TEST_CASE("Param ordering is correct every call") {
  REQUIRE(resultOf(R"(
    A = fn(a,b) {
      when {
        a eq 0 { false }
        a eq 5 { true }
        true { A(a+1, b) }
      }
    }

    A(1, 0)
  )")->asBool() == true);
}

TEST_CASE("Fns as arguments") {
  REQUIRE(resultOf(R"(
    do = fn (func, x) {
      func(x)
    }

    do(fn (x) { x + 1 }, 1)
  )")->asNumber() == fn::Number(0,2));
}

// TEST_CASE("Fn property get/set") {
//   REQUIRE(resultOf(R"(
//     x = fn (a) { a + 1 }
//     x.a = 1
//     x.a
//   )") == "1");
// }
