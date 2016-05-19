// Specs that test the system:
// Parser + Execution.

#include "src/ast.h"
#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

extern astBlock* programBlock;

std::string execute(const char code[]) {
  // Parse
  std::cout << code << std::endl;
  YY_BUFFER_STATE buffer = yy_scan_string(code);
  yyparse();
  yy_delete_buffer(buffer);

  // Execute
  fnMachine* context = new fnMachine();
  fnValue* returnValue = programBlock->execute(context);

  return returnValue->asString();
}

TEST_CASE("Semicolons") {
  std::string result = execute(
    "x = 1; x"
  );

  REQUIRE(result == "1");
}

TEST_CASE("Integer set/get") {
  std::string result = execute(
    "x = 1; x"
  );

  REQUIRE(result == "1");
}

TEST_CASE("Double set/get") {
  std::string result = execute(
    "x = 1.23; x"
  );

  REQUIRE(result == "1.230000");
}

TEST_CASE("String set/get") {
  std::string result = execute(
    "x = \"foo\"; x"
  );

  REQUIRE(result == "foo");
}

TEST_CASE("Boolean set/get") {
  std::string result = execute(
    "x = true; x"
  );

  REQUIRE(result == "true");
}

TEST_CASE("Block set/get") {
  std::string result = execute(
    "x = { a = 1 }; x.a"
  );

  REQUIRE(result == "1");
}

TEST_CASE("Fn set/get") {
  std::string result = execute(
    "x = (a) { a + 1 }; x(1)"
  );

  REQUIRE(result == "2");
}

