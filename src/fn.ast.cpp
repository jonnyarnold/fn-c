#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <algorithm>

#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

#define INT_SIZE (sizeof(int)*8)
#define CHAR_SIZE (sizeof(char)*8)

std::unique_ptr<Module> TheModule = make_unique<Module>("MAIN", getGlobalContext());
IRBuilder<> Builder(getGlobalContext());

std::vector<Type*> argTypes;
FunctionType* mainFnType =
  FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
Function* mainFn =
  Function::Create(mainFnType, Function::ExternalLinkage, "main", TheModule.get());

// Created to hold information
// about an assignment operation like
// x = 1
class assignment {
public:
  Value* value;
  Type* type;

  assignment(Value* value, Type* type) {
    this->value = value;
    this->type = type;
  }
};

std::map<std::string, assignment*> DefinedIds{};

#include "../src/fn.ast.h"

Value* astBlock::codegen() {
  std::cout << "astBlock\n";

  BasicBlock* block = BasicBlock::Create(getGlobalContext(), "block", mainFn);
  Builder.SetInsertPoint(block);

  Value* lastValue;

  // Statements are added to blocks in reverse order
  // by Flex/Bison.
  std::reverse(statements.begin(), statements.end());

  for(auto statement: statements) {
    lastValue = statement->codegen();
  }

  Builder.CreateRet(lastValue);

  return block;
}

Type* astBlock::type() {
  // CHECKME!
  return Type::getVoidTy(getGlobalContext());
}

std::string astId::fullyQualifiedName() {
  if(child == NULL) {
    return *name;
  }

  return *name + child->fullyQualifiedName();
}

Value* astId::codegen() {
  std::cout << "astId\n";

  auto id = DefinedIds.find(fullyQualifiedName().c_str());
  if(id == DefinedIds.end()) {
    std::cout << "Uh oh.";
    return NULL;
  }

  return Builder.CreateLoad(id->second->value, "");
}

Type* astId::type() {
  auto id = DefinedIds[fullyQualifiedName().c_str()];
  return id->type;
}

Value* astAssignment::codegen() {
  std::cout << "astAssignment\n";

  auto name = key->fullyQualifiedName().c_str();
  AllocaInst* alloc = Builder.CreateAlloca(value->type(), NULL, name);
  Value* valueCode = value->codegen();

  DefinedIds[name] = new assignment(alloc, value->type());
  Builder.CreateStore(valueCode, alloc);

  return valueCode;
}

Value* astInt::codegen() {
  std::cout << "astInt\n";
  return ConstantInt::get(getGlobalContext(), APInt(INT_SIZE, value));
}

Type* astInt::type() {
  return Type::getIntNTy(getGlobalContext(), INT_SIZE);
}

Value* astDouble::codegen() {
  std::cout << "astDouble\n";
  return ConstantFP::get(getGlobalContext(), APFloat(value));
}

Type* astDouble::type() {
  return Type::getDoubleTy(getGlobalContext());
}

Value* astString::codegen() {
  std::cout << "astString\n";
  return ConstantDataArray::getString(getGlobalContext(), value->c_str());
}

Type* astString::type() {
  return ArrayType::get(Type::getIntNTy(getGlobalContext(), CHAR_SIZE), strlen(value->c_str())+1);
}

Value* astBool::codegen() {
  std::cout << "astBool\n";
  return ConstantInt::get(getGlobalContext(), APInt(1, value));;
}

Type* astBool::type() {
  return Type::getInt1Ty(getGlobalContext());
}

Value* astFnCall::codegen() {
  std::cout << "astFnCall\n";
  return NULL;
}

Type* astFnCall::type() {
  // CHECKME!
  return Type::getVoidTy(getGlobalContext());
}

Value* astFnDef::codegen() {
  std::cout << "astFnDef\n";
  return NULL;
}

Type* astFnDef::type() {
  // CHECKME!
  return Type::getVoidTy(getGlobalContext());
}

Value* astCondition::codegen() {
  std::cout << "astCondition\n";
  return NULL;
}

Value* astConditional::codegen() {
  std::cout << "astConditional\n";
  return NULL;
}

Type* astConditional::type() {
  // CHECKME!
  return Type::getVoidTy(getGlobalContext());
}
