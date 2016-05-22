#include "parser.h"

#include <iostream>

astBlock* fnParser::parseFile(const char* filename) {
  yyin = fopen(filename,"r");
  yyparse();
  fclose(yyin);

  if(this->debug) { std::cout << programBlock->asString(0) << std::endl; }

  return programBlock;
}

astBlock* fnParser::parseCode(std::string code) {
  YY_BUFFER_STATE buffer = yy_scan_string(code.c_str());
  yyparse();
  yy_delete_buffer(buffer);

  if(this->debug) { std::cout << programBlock->asString(0) << std::endl; }

  return programBlock;
}