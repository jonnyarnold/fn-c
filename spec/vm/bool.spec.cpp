#include "spec/spec.h"
#include "src/vm/vm.h"

TEST_CASE("BOOL false") {
  fnByte instructions[1] = {FN_OP_FALSE};

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)1);
  REQUIRE(result.asBool == false);
}

TEST_CASE("BOOL true") {
  fnByte instructions[1] = {FN_OP_TRUE};

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)1);
  REQUIRE(result.asBool == true);
}

TEST_CASE("AND true true") {
  fnByte instructions[5] = {
    FN_OP_TRUE,
    FN_OP_TRUE,
    FN_OP_AND, 0, 1
  };

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == true);
}

TEST_CASE("AND true false") {
  fnByte instructions[5] = {
    FN_OP_TRUE,
    FN_OP_FALSE,
    FN_OP_AND, 0, 1
  };

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("AND false true") {
  fnByte instructions[5] = {
    FN_OP_FALSE,
    FN_OP_TRUE,
    FN_OP_AND, 0, 1
  };

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("AND false false") {
  fnByte instructions[5] = {
    FN_OP_FALSE,
    FN_OP_FALSE,
    FN_OP_AND, 0, 1
  };

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("NOT true") {
  fnByte instructions[3] = {
    FN_OP_TRUE,
    FN_OP_NOT, 0
  };

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)3);
  REQUIRE(result.asBool == false);
}

TEST_CASE("NOT false") {
  fnByte instructions[3] = {
    FN_OP_FALSE,
    FN_OP_NOT, 0
  };

  fnVM vm = fnVM();
  fnVMValue result = vm.run(instructions, (size_t)3);
  REQUIRE(result.asBool == true);
}
