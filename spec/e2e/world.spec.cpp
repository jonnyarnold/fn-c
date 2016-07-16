#include "spec/spec.h"

TEST_CASE("print exists") {
  REQUIRE(resultOf("print") == "(out) { ... }");
}

// TODO: include
//
// TEST_CASE("include") {

//   SECTION("adds to current scope when not assigned") {
//     REQUIRE(resultOf("include \"test-include.fn\"; imported") == "true");
//   }

//   SECTION("creates block when assigned") {
//     REQUIRE(resultOf("x = include \"test-include.fn\"; x.imported") == "true");
//   }
  
// }
