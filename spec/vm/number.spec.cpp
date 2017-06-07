#include "spec/spec.h"

using namespace fn;

TEST_CASE("NUMBER 1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(1, 0);

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("NUMBER 10") {
  bytecode::CodeBlob instructions = bytecode::iNumber(1, 1);

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == 1);
}

TEST_CASE("NUMBER -1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(-1, 0);

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -1);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("NUMBER 0.1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(1, -1);

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("NUMBER -0.1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(-1, -1);

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -1);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("MULTIPLY 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iMultiply()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 4);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("MULTIPLY 2 -2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(-2, 0),
    bytecode::iMultiply()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -4);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("MULTIPLY 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, 0),
    bytecode::iMultiply()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 4);
  REQUIRE(result.exponent == 1);
}

TEST_CASE("MULTIPLY 0.2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, -1),
    bytecode::iNumber(2, 0),
    bytecode::iMultiply()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 4);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("MULTIPLY -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 1),
    bytecode::iNumber(-2, -1),
    bytecode::iMultiply()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 4);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("DIVIDE 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iDivide()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("DIVIDE 2 -2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iDivide()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -1);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("DIVIDE 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, 1),
    bytecode::iDivide()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == 1);
}

// FIXME
// TEST_CASE("DIVIDE 10 4") {
//   bytecode::CodeBlob instructions = bytecode::CodeBlob{
//     bytecode::iNumber(1, 1),
//     bytecode::iNumber(4, 0),
//     bytecode::iDivide(1, 2)
//   };

//   Number result = resultOf(instructions)->asNumber();
//   REQUIRE(result.coefficient == 25);
//   REQUIRE(result.exponent == -1);
// }

TEST_CASE("DIVIDE 0.2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, -1),
    bytecode::iDivide()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("DIVIDE -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, -1),
    bytecode::iNumber(-2, 1),
    bytecode::iDivide()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 1);
  REQUIRE(result.exponent == 2);
}

TEST_CASE("ADD 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iAdd()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 4);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("ADD 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, 0),
    bytecode::iAdd()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 22);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("ADD 20 0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, -1),
    bytecode::iAdd()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 202);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("ADD -2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iAdd()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 0);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("ADD -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 1),
    bytecode::iNumber(-2, -1),
    bytecode::iAdd()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -202);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("SUBTRACT 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iSubtract()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 0);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("SUBTRACT 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(2, 1),
    bytecode::iSubtract()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 18);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("SUBTRACT 20 0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, -1),
    bytecode::iNumber(2, 1),
    bytecode::iSubtract()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == 198);
  REQUIRE(result.exponent == -1);
}

TEST_CASE("SUBTRACT -2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(-2, 0),
    bytecode::iSubtract()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -4);
  REQUIRE(result.exponent == 0);
}

TEST_CASE("SUBTRACT -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, -1),
    bytecode::iNumber(-2, 1),
    bytecode::iSubtract()
  };

  Number result = resultOf(instructions)->asNumber();
  REQUIRE(result.coefficient == -198);
  REQUIRE(result.exponent == -1);
}
