#include "spec/spec.h"

TEST_CASE("Boolean set/get") {
  REQUIRE(resultOf("x = true; x")->asBool() == true);
}

TEST_CASE("and") {
  REQUIRE(resultOf("true and true")->asBool() == true);
  REQUIRE(resultOf("true and false")->asBool() == false);
  REQUIRE(resultOf("false and true")->asBool() == false);
  REQUIRE(resultOf("false and false")->asBool() == false);
}

TEST_CASE("or") {
  REQUIRE(resultOf("true or true")->asBool() == true);
  REQUIRE(resultOf("true or false")->asBool() == true);
  REQUIRE(resultOf("false or true")->asBool() == true);
  REQUIRE(resultOf("false or false")->asBool() == false);
}

TEST_CASE("not") {
  REQUIRE(resultOf("not(true)")->asBool() == false);
  REQUIRE(resultOf("not(false)")->asBool() == true);
}

// TEST_CASE("Empty blocks are true") {
//   REQUIRE(resultOf("{} or false") == "true");
// }

TEST_CASE("0 is true") {
  REQUIRE(resultOf("0 or false")->asBool() == true);
  REQUIRE(resultOf("0 and true")->asBool() == true);
}

// TEST_CASE("Empty strings are true") {
//   REQUIRE(resultOf("\"\" or false") == "true");
// }
