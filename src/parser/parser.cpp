#include "src/parser/parser.h"

#include <iostream>

using namespace fn;

ast::Block* Parser::parseFile(const char* filename) {
  yyin = fopen(filename,"r");
  yyparse();
  fclose(yyin);

  if(this->debug) { std::cout << programBlock->asString(0) << std::endl; }
  this->parsedBlock = programBlock;

  return programBlock;
}

ast::Block* Parser::parseCode(std::string code) {
  YY_BUFFER_STATE buffer = yy_scan_string(code.c_str());
  yyparse();
  yy_delete_buffer(buffer);

  if(this->debug) { std::cout << programBlock->asString(0) << std::endl; }
  this->parsedBlock = programBlock;

  return programBlock;
}