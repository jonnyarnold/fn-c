#include "spec/spec.h"
#include "src/vm/vm.h"

TEST_CASE("NUMBER 1") {
  fnByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)0;
  instructions[2] = (fnCoefficient)1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("NUMBER 10") {
  fnByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)1;
  instructions[2] = (fnCoefficient)1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("NUMBER -1") {
  fnByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)0;
  instructions[2] = (fnCoefficient)-1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("NUMBER 0.1") {
  fnByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)-1;
  instructions[2] = (fnCoefficient)1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("NUMBER -0.1") {
  fnByte instructions[8];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)-1;
  instructions[2] = (fnCoefficient)-1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)8);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("MULTIPLY 2 2") {
  fnByte instructions[11];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)0;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_MULTIPLY;
  instructions[9] = 0;
  instructions[10] = 0;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)11);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("MULTIPLY 2 -2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)0;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)0;
  instructions[10] = (fnCoefficient)-2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == -4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("MULTIPLY 20 2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)1;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)0;
  instructions[10] = (fnCoefficient)2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("MULTIPLY 0.2 2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)-1;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)0;
  instructions[10] = (fnCoefficient)2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("MULTIPLY -20 -0.2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)1;
  instructions[2] = (fnCoefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)-1;
  instructions[10] = (fnCoefficient)-2;
  instructions[16] = FN_OP_MULTIPLY;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == 4);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 2 2") {
  fnByte instructions[11];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)0;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_DIVIDE;
  instructions[9] = 0;
  instructions[10] = 0;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)11);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 2 -2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)0;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)0;
  instructions[10] = (fnCoefficient)-2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == -1);
  REQUIRE(result.asNumber.exponent == 0);
}

TEST_CASE("DIVIDE 20 2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)1;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)0;
  instructions[10] = (fnCoefficient)2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 1);
}

TEST_CASE("DIVIDE 0.2 2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)-1;
  instructions[2] = (fnCoefficient)2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)0;
  instructions[10] = (fnCoefficient)2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == -1);
}

TEST_CASE("DIVIDE -20 -0.2") {
  fnByte instructions[18];
  instructions[0] = FN_OP_NUMBER;
  instructions[1] = (fnExponent)1;
  instructions[2] = (fnCoefficient)-2;
  instructions[8] = FN_OP_NUMBER;
  instructions[9] = (fnExponent)-1;
  instructions[10] = (fnCoefficient)-2;
  instructions[16] = FN_OP_DIVIDE;
  instructions[17] = 0;
  instructions[18] = 1;

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)18);
  REQUIRE(result.asNumber.coefficient == 1);
  REQUIRE(result.asNumber.exponent == 2);
}
