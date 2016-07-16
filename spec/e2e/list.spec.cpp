#include "spec/spec.h"

TEST_CASE("List get/set") {
  REQUIRE(resultOf("x = List(1, \"two\", true); x(1)") == "two");
}

TEST_CASE("List.map") {
  REQUIRE(resultOf(R"(
    l = List(1, 2, 3)
    l.map(fn (i) { i + 1 })
  )") == "List(2, 3, 4)");
}
