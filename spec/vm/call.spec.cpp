#include "spec/spec.h"

TEST_CASE("CALL builtin") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("CALL def") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(2, std::vector<bytecode::NameHash>{}), 
    bytecode::iFalse(),
    bytecode::iReturnLast(),
    bytecode::iCall()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("CALL def with args") {
  bytecode::NameHash x = bytecode::hashName("x");

  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iDefHeader(3, std::vector<bytecode::NameHash>{x}),
    bytecode::iLoad(x),      
    bytecode::iReturnLast(),
    bytecode::iCall()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("CALL def with recursion") {
  // This is the equivalent of:
  // x = fn(a) { 
  //   when {
  //     a { a }
  //     true { self(true) }
  // }
  //
  // x(false)
  bytecode::NameHash a = bytecode::hashName("a");
  bytecode::NameHash x = bytecode::hashName("x");
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(13, std::vector<bytecode::NameHash>{a}),
    
    bytecode::iLoad(a),
    bytecode::iJumpIfLastFalse(3),
    bytecode::iLoad(a),
    bytecode::iReturnLast(),
    
    bytecode::iTrue(),
    bytecode::iJumpIfLastFalse(3),
    bytecode::iLoad(x),
    bytecode::iCall(),
    bytecode::iReturnLast(),

    bytecode::iReturnLast(),

    bytecode::iName("x"),
    bytecode::iTrue(),
    bytecode::iLoad(x),
    bytecode::iCall()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}
