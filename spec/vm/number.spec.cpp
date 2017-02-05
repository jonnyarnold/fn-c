#include "spec/spec.h"
#include "src/vm/vm.h"

using namespace fn;

TEST_CASE("NUMBER 1") {
  bytecode::CodeByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("NUMBER 10") {
  bytecode::CodeByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("NUMBER -1") {
  bytecode::CodeByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)-1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("NUMBER 0.1") {
  bytecode::CodeByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)-1;
  instructions[2] = (vm::number::Coefficient)1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("NUMBER -0.1") {
  bytecode::CodeByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)-1;
  instructions[2] = (vm::number::Coefficient)-1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("MULTIPLY 2 2") {
  bytecode::CodeByte instructions[11];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_MULTIPLY;
  instructions[9] = 0;
  instructions[10] = 0;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)11);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("MULTIPLY 2 -2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)-2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == -4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("MULTIPLY 20 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("MULTIPLY 0.2 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)-1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("MULTIPLY -20 -0.2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)-1;
  instructions[10] = (vm::number::Coefficient)-2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 2 2") {
  bytecode::CodeByte instructions[11];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_DIVIDE;
  instructions[9] = 0;
  instructions[10] = 0;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)11);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 2 -2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)-2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 20 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("DIVIDE 0.2 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)-1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("DIVIDE -20 -0.2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)-1;
  instructions[10] = (vm::number::Coefficient)-2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 2);
}

TEST_CASE("ADD 2 2") {
  bytecode::CodeByte instructions[11];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_ADD;
  instructions[9] = 0;
  instructions[10] = 0;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)11);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("ADD 20 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_ADD;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 22);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("ADD 20 0.2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)-1;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_ADD;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 202);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("ADD -2 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_ADD;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 0);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("ADD -20 -0.2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)-1;
  instructions[10] = (vm::number::Coefficient)-2;
  instructions[16] = FN_OP_ADD;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == -202);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("SUBTRACT 2 2") {
  bytecode::CodeByte instructions[11];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_SUBTRACT;
  instructions[9] = 0;
  instructions[10] = 0;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)11);
  REQUIRE(result.asNumber.coefficient == 0);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("SUBTRACT 20 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_SUBTRACT;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 18);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("SUBTRACT 20 0.2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)-1;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_SUBTRACT;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == 198);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("SUBTRACT -2 2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)0;
  instructions[2] = (vm::number::Coefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)0;
  instructions[10] = (vm::number::Coefficient)2;
  instructions[16] = FN_OP_SUBTRACT;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == -4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("SUBTRACT -20 -0.2") {
  bytecode::CodeByte instructions[19];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (vm::number::Exponent)1;
  instructions[2] = (vm::number::Coefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (vm::number::Exponent)-1;
  instructions[10] = (vm::number::Coefficient)-2;
  instructions[16] = FN_OP_SUBTRACT;
  instructions[17] = 0;
  instructions[18] = 1;

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)19);
  REQUIRE(result.asNumber.coefficient == -198);
  REQUIRE(result.asNumber.exponent == -1);
}
