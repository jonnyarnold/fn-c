#pragma once

#include "src/ast.h"

// Required extern declarations
// linking to Flex/Bison.
extern FILE* yyin;
extern int yyparse();
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern astBlock* programBlock;

// Converts fn code into an Abstract Syntax Tree (AST).
class fnParser {
  bool debug;

public:
  fnParser(bool debug) {
    this->debug = debug;
  }

  fnParser() : fnParser(false) {}

  // Return the AST for the given file.
  astBlock* parseFile(const char* filename);

  // Return the AST for the given code.
  astBlock* parseCode(std::string code);
};
