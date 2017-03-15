#include "src/parser/parser.h"

#include <iostream>

using namespace fn;

ast::Block* Parser::parseFile(const char* filename) {
  if (programBlock != NULL) { delete programBlock; }

  yyin = fopen(filename,"r");
  yyparse();
  fclose(yyin);

  if(this->debug) { std::cout << programBlock->asString(0) << std::endl; }

  return programBlock;
}

ast::Block* Parser::parseCode(std::string code) {
  if (programBlock != NULL) { delete programBlock; }
  
  YY_BUFFER_STATE buffer = yy_scan_string(code.c_str());
  yyparse();
  yy_delete_buffer(buffer);

  if(this->debug) { std::cout << programBlock->asString(0) << std::endl; }

  return programBlock;
}