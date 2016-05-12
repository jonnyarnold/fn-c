#include <iostream>
#include <stack>

#include "ast.h"
#include "interpreter/runtime.h"
#include "interpreter/builtins.h"

extern int yyparse();

extern astBlock* programBlock;

int main(int argc, char **argv)
{
  yyparse();
  std::cout << "\n\n\n";

  fnExecution* context = new fnExecution();
  context->blockStack->push(new fnTopBlock());

  fnValue* returnValue = programBlock->execute(context);

  std::cout << static_cast<fnInt*>(returnValue)->value;

  return 0;
}
