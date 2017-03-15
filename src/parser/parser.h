// The parser provides a C++-friendly interface
// to the C-based Flex/Bison parser.

#pragma once

#include "src/ast.h"

// These extern declarations are required
// by Flex/Bison.
extern FILE* yyin;
extern int yyparse();
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern fn::ast::Block* programBlock;

namespace fn { 

  class Parser {
    bool debug;
    ast::Block* parsedBlock;

  public:
    Parser(bool debug) { 
      this->debug = debug; 
      this->parsedBlock = NULL;
    }
    Parser() : Parser(false) {}
    
    ~Parser() {
      if (this->parsedBlock != NULL) { delete this->parsedBlock; }
    }

    // Return the AST for the given file.
    ast::Block* parseFile(const char* filename);

    // Return the AST for the given code.
    ast::Block* parseCode(std::string code);
  };

}
