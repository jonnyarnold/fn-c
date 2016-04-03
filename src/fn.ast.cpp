#include <string>
#include <vector>
#include <iostream>

#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

std::unique_ptr<Module> TheModule = make_unique<Module>("MAIN", getGlobalContext());

#include "../src/fn.ast.h"

Value* astBlock::codegen() {
  std::cout << "astBlock\n";
  std::cout << statements.size();
  for(auto statement: statements) {
    statement->codegen();
  }

  return NULL;
}

Value* astId::codegen() {
  std::cout << "astId\n";
  return NULL;
}

Value* astAssignment::codegen() {
  std::cout << "astAssignment\n";
  return NULL;
}

Value* astInt::codegen() {
  std::cout << "astInt\n";
  return ConstantInt::get(getGlobalContext(), APInt(value, sizeof(int)));
}

Value* astDouble::codegen() {
  std::cout << "astDouble\n";
  return ConstantFP::get(getGlobalContext(), APFloat(value));
}

Value* astString::codegen() {
  std::cout << "astString\n";
  return NULL;
}

Value* astBool::codegen() {
  std::cout << "astBool\n";
  return NULL;
}

Value* astFnCall::codegen() {
  std::cout << "astFnCall\n";
  return NULL;
}

Value* astFnDef::codegen() {
  std::cout << "astFnDef\n";
  return NULL;
}

Value* astCondition::codegen() {
  std::cout << "astCondition\n";
  return NULL;
}

Value* astConditional::codegen() {
  std::cout << "astConditional\n";
  return NULL;
}
