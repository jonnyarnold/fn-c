#include <iostream>
#include <stack>

#include "../src/fn.ast.h"
#include "../src/fn.runtime.h"

extern int yyparse();

extern astBlock* programBlock;

int main(int argc, char **argv)
{
  yyparse();
  std::cout << "\n\n\n";

  fnExecution* context = new fnExecution();
  fnValue* returnValue = programBlock->execute(context);

  static_cast<fnString*>(returnValue)->dump();

  return 0;
}
