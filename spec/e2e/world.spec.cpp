#include "spec/spec.h"

TEST_CASE("print exists") {
  REQUIRE(resultOf("print") == "(out) { ... }");
}

// TODO: Better path resolution
//
// TEST_CASE("load sets value from other file") {
//   REQUIRE(resultOf("x = load(\"test-include.fn\"); x.imported") == "true");
// }

TEST_CASE("include") {

  SECTION("adds to current scope from value") {
    REQUIRE(resultOf("include({ imported = true }); imported") == "true");
  }

  SECTION("creates block when assigned externally") {
    REQUIRE(resultOf(R"(
      x = {}
      x.include({ imported = true })
      x.imported
    )") == "true");
  }
  
}
