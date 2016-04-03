#include <iostream>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "../src/fn.ast.h"

extern int yyparse();
extern astBlock* programBlock;
extern std::unique_ptr<llvm::Module> TheModule;

int main(int argc, char **argv)
{
  yyparse();

  // Make the module, which holds all the code.
  programBlock->codegen();

  // Print out all of the generated code.
  // TheModule->dump();

  return 0;
}
