#include <iostream>
#include <algorithm>

#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

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

  std::cout << "!!! undefined: " << *name << "\n";
  return NULL;
}

fnBlock* fnBlock::getBlockById(std::string* id) {
  return static_cast<fnBlock*>(this->get(id));
}

fnCallable* fnBlock::getDefById(std::string* id) {
  return static_cast<fnCallable*>(this->get(id));
}

fnValue* fnDef::call(fnMachine* context, std::vector<fnValue*> args) {
  fnBlock* block = context->pushNewBlock(this->parentBlock);

  // Set the arguments in the FnDef block.
  // Note that args are given to us in reverse!
  std::reverse(args.begin(), args.end());
  for(int i = 0; i < this->params->size(); i++) {
    std::string paramName = (*this->params)[i];
    fnValue* paramValue = args[i];

    context->setValue(&paramName, paramValue);
  }

  // Execute the FnDef instructions.
  fnValue* result = this->func(context, args);

  context->popBlock();

  return result;
}
