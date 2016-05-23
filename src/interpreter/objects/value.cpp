#include "src/interpreter/objects/value.h"

#include <iostream>

fnValue* fnValue::get(std::string* name) {
  fnValue* value = this->locals[(*name)];

  if(value == NULL) {
    // TODO: fnError
    std::cout << "RuntimeError(\"" << *name << " is not defined.\")\n";
    exit(-1);
  }

  return value;
}
