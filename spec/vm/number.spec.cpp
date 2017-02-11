#include "spec/spec.h"

using namespace fn;

TEST_CASE("NUMBER 1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(1, 0);

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("NUMBER 10") {
  bytecode::CodeBlob instructions = bytecode::iNumber(1, 1);

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("NUMBER -1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(-1, 0);

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("NUMBER 0.1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(1, -1);

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("NUMBER -0.1") {
  bytecode::CodeBlob instructions = bytecode::iNumber(-1, -1);

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("MULTIPLY 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iMultiply(0, 0)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("MULTIPLY 2 -2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(-2, 0),
    bytecode::iMultiply(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("MULTIPLY 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, 0),
    bytecode::iMultiply(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("MULTIPLY 0.2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, -1),
    bytecode::iNumber(2, 0),
    bytecode::iMultiply(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("MULTIPLY -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 1),
    bytecode::iNumber(-2, -1),
    bytecode::iMultiply(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iDivide(0, 0)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 2 -2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iNumber(-2, 0),
    bytecode::iDivide(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, 0),
    bytecode::iDivide(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("DIVIDE 0.2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, -1),
    bytecode::iNumber(2, 0),
    bytecode::iDivide(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("DIVIDE -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 1),
    bytecode::iNumber(-2, -1),
    bytecode::iDivide(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 2);
}

TEST_CASE("ADD 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iAdd(0, 0)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("ADD 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, 0),
    bytecode::iAdd(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 22);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("ADD 20 0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, -1),
    bytecode::iAdd(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 202);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("ADD -2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iAdd(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 0);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("ADD -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 1),
    bytecode::iNumber(-2, -1),
    bytecode::iAdd(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -202);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("SUBTRACT 2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 0),
    bytecode::iSubtract(0, 0)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 0);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("SUBTRACT 20 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, 0),
    bytecode::iSubtract(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 18);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("SUBTRACT 20 0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(2, 1),
    bytecode::iNumber(2, -1),
    bytecode::iSubtract(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == 198);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("SUBTRACT -2 2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 0),
    bytecode::iNumber(2, 0),
    bytecode::iSubtract(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("SUBTRACT -20 -0.2") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(-2, 1),
    bytecode::iNumber(-2, -1),
    bytecode::iSubtract(0, 1)
  };

  vm::Value result = resultOf(instructions);
  REQUIRE(result.asNumber.coefficient == -198);
  REQUIRE(result.asNumber.exponent == -1);
}
