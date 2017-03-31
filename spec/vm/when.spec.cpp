#include "spec/spec.h"

TEST_CASE("WHEN") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iWhenHeader(12),

    bytecode::iFalse(),
    bytecode::iJumpIfLastFalse(2),
    bytecode::iTrue(),
    bytecode::iReturnLast(),

    bytecode::iTrue(),
    bytecode::iJumpIfLastFalse(2),
    bytecode::iFalse(),
    bytecode::iReturnLast()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}
