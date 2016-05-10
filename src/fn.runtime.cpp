#include <iostream>

#include "fn.runtime.h"

void fnBlock::set(std::string* name, fnValue* value) {
  this->locals[(*name)] = value;
}

fnValue* fnBlock::get(std::string* name) {
  fnValue* value = this->locals[(*name)];
  if(value != NULL) {
    return value;
  }

  if(this->parent != NULL) {
    return this->parent->get(name);
  }

  std::cout << "!!! undefined: " << name << "\n";
  return NULL;
}

fnBlock* fnBlock::getBlockById(std::string* id) {
  return static_cast<fnBlock*>(this->get(id));
}

fnDef* fnBlock::getDefById(std::string* id) {
  return static_cast<fnDef*>(this->get(id));
}

fnValue* fnDef::call(fnExecution* context, std::vector<fnValue*> args) {
  // Create a new block
  std::cout << "PUSH_NEW_BLOCK(parent " << this->parentBlock << ")";
  fnBlock* block = new fnBlock(this->parentBlock);
  context->blockStack->push(block);
  std::cout << " = " << block << "\n";

  // Set the arguments in the FnDef block.
  for(int i = 0; i < this->params->size(); i++) {
    std::string paramName = (*this->params)[i];
    fnValue* paramValue = args[i];

    std::cout << "SET_VALUE(name " << paramName << ", value " << paramValue << ")\n";
    context->currentBlock()->set(&paramName, paramValue);
  }

  // Execute the FnDef instructions.
  fnValue* result = this->func(context, args);

  context->blockStack->pop();

  std::cout << "RETURN_TO_BLOCK(" << this->parentBlock << ")\n";
  return result;
}
