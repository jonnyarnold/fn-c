#include "spec/spec.h"

TEST_CASE("BOOL false") {
  bytecode::CodeBlob instructions = bytecode::iFalse();
  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("BOOL true") {
  bytecode::CodeBlob instructions = bytecode::iTrue();
  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("AND true true") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iTrue(),
    bytecode::iAnd()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("AND true false") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iFalse(),
    bytecode::iAnd()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("AND false true") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse(),
    bytecode::iTrue(),
    bytecode::iAnd()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("AND false false") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse(),
    bytecode::iFalse(),
    bytecode::iAnd()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("OR true true") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iTrue(),
    bytecode::iOr()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("OR true false") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iFalse(),
    bytecode::iOr()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("OR false true") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse(),
    bytecode::iTrue(),
    bytecode::iOr()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("OR false false") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse(),
    bytecode::iFalse(),
    bytecode::iOr()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("NOT true") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iNot()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("NOT false") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iFalse(),
    bytecode::iNot()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}
