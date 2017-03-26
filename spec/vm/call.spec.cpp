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
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(3), // V1
    bytecode::iLoad(1),      // V1.V1 = arg1
    bytecode::iReturnLast(),
    bytecode::iTrue(), // V2
    bytecode::iCall(1, 1, 2) // fn = V1, 1 arg = V2
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}
