#include "spec/spec.h"

TEST_CASE("LOAD") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iLoad(0)
  };

  REQUIRE(resultOf(instructions).asBool == true);
}
