#include "spec/spec.h"

TEST_CASE("CALL builtin") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse()
  };

  REQUIRE(resultOf(instructions).asBool() == false);
}

TEST_CASE("CALL def") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(2),
    bytecode::iFalse(),
    bytecode::iReturnLast(),
    bytecode::iCall(0)
  };

  REQUIRE(resultOf(instructions).asBool() == false);
}
