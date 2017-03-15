#include "spec/spec.h"

// TEST_CASE("Block set/get") {
//   REQUIRE(resultOf("x = {}; x") == "{}");
//   REQUIRE(resultOf("x = { a = 1 }; x.a") == "1");
// }

// TEST_CASE("Assignment outside of block") {
//   REQUIRE(resultOf("x = {}; x.y = 1; x.y") == "1");
// }

// TEST_CASE("Nested blocks") {
//   REQUIRE(resultOf(R"(
//     Utils = {
//       Math = {
//         pi = 3
//         area = fn (diameter) { pi * diameter }
//       }
//     }

//     Utils.Math.area(3)
//   )") == "9");
// }

// TODO: .keys
//
// TEST_CASE(".keys") {
//   REQUIRE(resultOf(R"(
//     x = { a = 1; b = 2 }
//     x.keys
//   )") == "List(\"a\", \"b\")");
// }

// TODO: .values
//
// TEST_CASE(".values") {
//   REQUIRE(resultOf(R"(
//     x = { a = 1; b = 2 }
//     x.values
//   )") == "List(1,2)");
// }
