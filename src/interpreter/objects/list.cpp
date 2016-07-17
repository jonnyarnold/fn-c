#include "src/interpreter/objects/list.h"

#include <iostream>

fnValue* fnMap::execute(fnMachine* context, std::vector<fnValue*> values) {
  fnDef* def = dynamic_cast<fnDef*>(values[0]);
  if (def == NULL) {
    std::cout << "RuntimeError(non-def " << values[0]->asString() << " given to map)";
    exit(-1);
  }

  std::vector<fnValue*>* results = new std::vector<fnValue*>();
  for(auto value: this->list->values) {
    fnValue* result = context->callByValue(def, std::vector<fnValue*> { value });
    results->push_back(result);
  }

  
  return new fnList(*results);
}