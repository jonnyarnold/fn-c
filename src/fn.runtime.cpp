#include <iostream>

#include "fn.runtime.h"

void fnBlock::set(std::string* name, fnValue* value) {
  this->locals[(*name)] = value;
}

fnValue* fnBlock::get(std::string* name) {
  return this->locals[(*name)];
}

fnBlock* fnBlock::getBlockById(std::string* id) {
  return static_cast<fnBlock*>(this->locals[(*id)]);
}

fnDef* fnBlock::getDefById(std::string* id) {
  return static_cast<fnDef*>(this->locals[(*id)]);
}

fnValue* fnDef::call(fnExecution* context, std::vector<fnValue*> args) {
  // Create a new block
  std::cout << "PUSH_NEW_BLOCK " << this->parentBlock << "\n";
  fnBlock* block = new fnBlock(this->parentBlock);

  // Set the arguments in the FnDef block.
  for(int i = 0; i < this->params->size(); i++) {
    std::string* paramName = (*this->params)[i];
    fnValue* paramValue = args[i];

    std::cout << "SET " << (*paramName) << "\n";
    context->currentBlock()->set(paramName, paramValue);
  }

  // Execute the FnDef instructions.
  fnValue* result = this->value(args);

  context->blockStack->pop();

  std::cout << "RETURN " << this->parentBlock << "\n";
  return result;
}
