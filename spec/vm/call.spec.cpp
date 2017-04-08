#include "spec/spec.h"

TEST_CASE("CALL builtin") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("CALL def") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(2), // V1
    bytecode::iFalse(),      // V1.V1
    bytecode::iReturnLast(),
    bytecode::iCall(1)
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("CALL def with args") {
  bytecode::ValueIndex args[] = {2};

  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(3), // V1
    bytecode::iLoad(1),      // V1.V1 = arg1
    bytecode::iReturnLast(),
    bytecode::iTrue(), // V2
    bytecode::iCall(1, 1, args) // fn = V1, 1 arg = V2
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("CALL def with recursion") {
  bytecode::ValueIndex outerArgs[] = {2};
  bytecode::ValueIndex innerArgs[] = {4};

  // This is the equivalent of:
  // x = fn(a) { 
  //   when {
  //     a { a }
  //     true { self(true) }
  // }
  //
  // x(false)
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(16), // V1 (and V1.V1)
    
    bytecode::iWhenHeader(13), // V1.V3
    
    bytecode::iLoad(2), // V1.V2 = arg1
    bytecode::iJumpIfLastFalse(3),
    bytecode::iLoad(2), // V1.V3 = arg1
    bytecode::iReturnLast(),
    
    bytecode::iTrue(), // V1.V4
    bytecode::iCall(1, 1, innerArgs), // fn = V1.V1, 1 arg = V1.V4
    bytecode::iReturnLast(),

    bytecode::iReturnLast(),

    bytecode::iTrue(), // V2
    bytecode::iCall(1, 1, outerArgs) // fn = V1, 1 arg = V2
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}
