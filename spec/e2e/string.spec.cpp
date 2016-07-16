#include "spec/spec.h"

TEST_CASE("String set/get") {
  REQUIRE(resultOf("x = \"foo\"; x") == "foo");
}
