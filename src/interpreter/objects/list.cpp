#include "src/interpreter/objects/list.h"

#include <iostream>

fnValue* map(fnMachine* context, std::vector<fnValue*> values) {
  fnList* list = dynamic_cast<fnList*>(context->currentScope()->parent);
  if (list == NULL) {
    std::cout << "RuntimeError(map called with non-list parent " << context->currentScope()->asString() << ")";
    exit(-1);
  }

  fnDef* def = dynamic_cast<fnDef*>(values[0]);
  if (def == NULL) {
    std::cout << "RuntimeError(non-def " << values[0]->asString() << " given to map)";
    exit(-1);
  }

  std::vector<fnValue*>* results = new std::vector<fnValue*>();
  for(auto value: list->values) {
    fnValue* result = context->callByValue(def, std::vector<fnValue*> { value });
    results->push_back(result);
  }

  
  return new fnList(*results);
}