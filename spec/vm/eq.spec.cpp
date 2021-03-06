#include "spec/spec.h"

TEST_CASE("EQ bool bool (different)") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iFalse(),
    bytecode::iEq()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("EQ bool bool (value equal)") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iTrue(),
    bytecode::iTrue(),
    bytecode::iEq()
  };

  REQUIRE(resultOf(instructions)->asBool() == true);
}

TEST_CASE("EQ number number (different)") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iNumber(5, 6),
    bytecode::iNumber(6, 5),
    bytecode::iEq()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("EQ def def (different)") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(2, std::vector<bytecode::NameHash>{}),
    bytecode::iTrue(),
    bytecode::iReturnLast(),
    bytecode::iDefHeader(2, std::vector<bytecode::NameHash>{}),
    bytecode::iFalse(),
    bytecode::iReturnLast(),
    bytecode::iEq()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

TEST_CASE("EQ def def (value equal IS FALSE)") {
  bytecode::CodeBlob instructions = bytecode::CodeBlob{
    bytecode::iDefHeader(2, std::vector<bytecode::NameHash>{}),
    bytecode::iTrue(),
    bytecode::iReturnLast(),
    bytecode::iDefHeader(2, std::vector<bytecode::NameHash>{}),
    bytecode::iTrue(),
    bytecode::iReturnLast(),
    bytecode::iEq()
  };

  REQUIRE(resultOf(instructions)->asBool() == false);
}

// TEST_CASE("EQ def def (reference equal IS TRUE)") {
//   bytecode::CodeBlob instructions = bytecode::CodeBlob{
//     bytecode::iDefHeader(2),
//     bytecode::iTrue(),
//     bytecode::iReturnLast(),
//     bytecode::iEq()
//   };

//   REQUIRE(resultOf(instructions)->asBool() == true);
// }
