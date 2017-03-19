#include "spec/spec.h"

TEST_CASE("Boolean set/get") {
  REQUIRE(resultOf("x = true; x")->asBool() == true);
}

// TEST_CASE("and") {
//   REQUIRE(resultOf("true and true") == "true");
//   REQUIRE(resultOf("true and false") == "false");
//   REQUIRE(resultOf("false and true") == "false");
//   REQUIRE(resultOf("false and false") == "false");
// }

// TEST_CASE("or") {
//   REQUIRE(resultOf("true or true") == "true");
//   REQUIRE(resultOf("true or false") == "true");
//   REQUIRE(resultOf("false or true") == "true");
//   REQUIRE(resultOf("false or false") == "false");
// }

// TEST_CASE("not") {
//   REQUIRE(resultOf("not(true)") == "false");
//   REQUIRE(resultOf("not(false)") == "true");
// }

// TEST_CASE("Empty blocks are true") {
//   REQUIRE(resultOf("{} or false") == "true");
// }

// TEST_CASE("0 is true") {
//   REQUIRE(resultOf("0 or false") == "true");
// }

// TEST_CASE("Empty strings are true") {
//   REQUIRE(resultOf("\"\" or false") == "true");
// }
