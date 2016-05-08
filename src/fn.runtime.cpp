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
