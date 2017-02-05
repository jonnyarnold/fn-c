#include "spec/spec.h"
#include "src/vm/vm.h"

using namespace fn;

TEST_CASE("BOOL false") {
  bytecode::CodeByte instructions[1] = {FN_OP_FALSE};

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)1);
  REQUIRE(result.asBool == false);
}

TEST_CASE("BOOL true") {
  bytecode::CodeByte instructions[1] = {FN_OP_TRUE};

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)1);
  REQUIRE(result.asBool == true);
}

TEST_CASE("AND true true") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_TRUE,
    FN_OP_TRUE,
    FN_OP_AND, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == true);
}

TEST_CASE("AND true false") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_TRUE,
    FN_OP_FALSE,
    FN_OP_AND, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("AND false true") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_FALSE,
    FN_OP_TRUE,
    FN_OP_AND, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("AND false false") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_FALSE,
    FN_OP_FALSE,
    FN_OP_AND, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("OR true true") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_TRUE,
    FN_OP_TRUE,
    FN_OP_OR, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == true);
}

TEST_CASE("OR true false") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_TRUE,
    FN_OP_FALSE,
    FN_OP_OR, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == true);
}

TEST_CASE("OR false true") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_FALSE,
    FN_OP_TRUE,
    FN_OP_OR, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == true);
}

TEST_CASE("OR false false") {
  bytecode::CodeByte instructions[5] = {
    FN_OP_FALSE,
    FN_OP_FALSE,
    FN_OP_OR, 0, 1
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)5);
  REQUIRE(result.asBool == false);
}

TEST_CASE("NOT true") {
  bytecode::CodeByte instructions[3] = {
    FN_OP_TRUE,
    FN_OP_NOT, 0
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)3);
  REQUIRE(result.asBool == false);
}

TEST_CASE("NOT false") {
  bytecode::CodeByte instructions[3] = {
    FN_OP_FALSE,
    FN_OP_NOT, 0
  };

  VM vm = VM();
  vm::Value result = vm.run(instructions, (size_t)3);
  REQUIRE(result.asBool == true);
}
