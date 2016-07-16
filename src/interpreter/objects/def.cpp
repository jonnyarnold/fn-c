#include "src/interpreter/objects/def.h"

#include "src/interpreter/machine.h"

fnValue* fnDef::call(fnMachine* context, std::vector<fnValue*> args) {
  fnValue* scope = context->pushNewScope(this->parentScope);

  // Set the arguments in the FnDef scope.
  for(int i = 0; i < this->params->size(); i++) {
    std::string paramName = (*this->params)[i];
    fnValue* paramValue = args[i];

    context->setValue(&paramName, paramValue);
  }

  // Execute the FnDef instructions.
  fnValue* result = this->func(context, args);

  context->popScope();

  return result;
}

std::string fnDef::asString(int indentationLevel) {
  std::string result = "(";
  for(auto param: (*this->params)) {
    result += param;
    if(param != this->params->back()) {
      result += ", ";
    }
  }

  result += ") { ... }";

  return result;
}
