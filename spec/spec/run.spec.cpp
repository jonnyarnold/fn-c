// Specs that test the system:
// Parser + Execution.

#include "src/parser.h"
#include "src/interpreter/machine.h"

std::string value(const char code[]) {
  astBlock* program = (new fnParser())->parseCode(code);
  fnMachine* context = new fnMachine();
  fnValue* value = program->execute(context);

  return value->asString();
}

TEST_CASE("Empty programs compile") {
  REQUIRE(value("") == "{}");
}

TEST_CASE("Semicolons") {
  REQUIRE(value("x = 1; x") == "1");
}

TEST_CASE("Integer set/get") {
  REQUIRE(value("x = 1; x") == "1");
}

TEST_CASE("Double set/get") {
  REQUIRE(value("x = 1.23; x") == "1.230000");
}

TEST_CASE("String set/get") {
  REQUIRE(value("x = \"foo\"; x") == "foo");
}

TEST_CASE("Boolean set/get") {
  REQUIRE(value("x = true; x") == "true");
}

TEST_CASE("Block set/get") {
  REQUIRE(value("x = {}; x") == "{}");
  REQUIRE(value("x = { a = 1 }; x.a") == "1");
}

TEST_CASE("Fn set/call") {
  REQUIRE(value("x = fn (a) { a + 1 }; x(1)") == "2");
}

TEST_CASE("Fn set/call in block") {
  REQUIRE(value(R"(
    x = {
      y = fn (a) { a + 1 }
    }

    x.y(1)
  )") == "2");
}

TEST_CASE("Assignment outside of block") {
  REQUIRE(value("x = {}; x.y = 1; x.y") == "1");
}

TEST_CASE("Fn call within block") {
  REQUIRE(value(R"(
    x = fn(a) { 
      foo = fn(b) { a + b }
    }
    
    x(1).foo(1)
  )") == "2");
}

TEST_CASE("Brackets") {
  REQUIRE(value("2 * (1 + 2)") == "6");
}

TEST_CASE("Param ordering is correct every call") {
  REQUIRE(value(R"(
    A = fn(a,b) {
      when {
        a eq 0 { false }
        a eq 5 { true }
        true { A(a+1, b) }
      }
    }

    A(1, 0)
  )") == "true");
}

