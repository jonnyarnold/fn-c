#include "src/parser.h"
#include "src/interpreter/machine.h"

fnValue* exec(const char fileName[], bool debug) {
  astBlock* program = (new fnParser(debug))->parseFile(fileName);

  fnMachine* context = new fnMachine(debug);
  return program->execute(context);
}
