#include "spec/spec.h"

TEST_CASE("NEW_FRAME COMPRESS returns to original frame") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iNewFrame(),
    bytecode::iCompress()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("NEW_FRAME COMPRESS EXPAND uses new frame") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNewFrame(),
    bytecode::iTrue(),
    bytecode::iCompress(),
    bytecode::iExpand()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("Dereference") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNewFrame(),
    bytecode::iTrue(),
    bytecode::iCompress(),
    bytecode::iExpand(),
    bytecode::iReturnLast()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}
