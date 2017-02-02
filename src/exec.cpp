#include "src/parser.h"
#include "src/interpreter/machine.h"

fnValue* exec(const char fileName[], bool debug) {
  fnParser* parser = new fnParser(debug);
  astBlock* program = parser->parseFile(fileName);

  fnMachine* context = new fnMachine(debug);
  fnValue* result = program->execute(context);

  delete context;
  delete program;
  delete parser;
  
  return result;
}
