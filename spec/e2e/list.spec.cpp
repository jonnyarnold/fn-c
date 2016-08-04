#include "spec/spec.h"

TEST_CASE("List get/set") {
  REQUIRE(resultOf("x = List(1, \"two\", true); x(1)") == "two");
}

TEST_CASE("List.map") {

  SECTION("applies to every item of the List") {
    REQUIRE(resultOf(R"(
      l = List(1, 2, 3)
      l.map(fn (i) { i + 1 })
    )") == "List(2, 3, 4)");
  }
  
  SECTION("Fails if non-def is given") {
    REQUIRE(failure(R"(
      l = List(1, 2, 3)
      l.map(false)
    )"));
  }

}

// TODO: List.reduce
//
// TEST_CASE("List.reduce") {
//   REQUIRE(resultOf(R"(
//     l = List(1, 2, 3)
//     l.reduce(0, fn (memo, item) { memo + item })
//   )") == "6");
// }

// TODO: A decent Enumerable method set!
