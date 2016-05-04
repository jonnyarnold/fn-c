#include <iostream>
#include <stack>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "../src/fn.ast.h"

extern int yyparse();

extern astBlock* programBlock;

extern std::unique_ptr<llvm::Module> TheModule;
extern llvm::Function* mainFn;

int main(int argc, char **argv)
{
  yyparse();

  std::cout << "\n\n\n";

  // Make the module, which holds all the code.
  programBlock->codegen();

  // Print out all of the generated code.
  TheModule->dump();

  return 0;
}
