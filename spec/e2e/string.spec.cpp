#include "spec/spec.h"

TEST_CASE("String set/get") {
  REQUIRE(resultOf("x = \"foo\"; x") == "foo");
}

// TODO: .length
//
// TEST_CASE(".length") {
//   REQUIRE(resultOf("(\"foo\").length") == "3");
// }
