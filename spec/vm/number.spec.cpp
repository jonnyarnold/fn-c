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
