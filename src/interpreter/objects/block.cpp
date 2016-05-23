#include "src/interpreter/objects/block.h"

#include <iostream>

// Number of spaces between indent levels when converting to string.
const int INDENT_SIZE = 2;

fnValue* fnBlock::get(std::string* name) {
  fnValue* value = this->locals[(*name)];

  if(value == NULL) {
    if(this->parent != NULL) { return this->parent->get(name); }
    else {
      // TODO: fnError
      std::cout << "RuntimeError(\"" << *name << " is not defined.\")\n";
      exit(-1);
    }
  }

  return value;
}

template<typename T>
T fnBlock::get(std::string* name) {
  T value = dynamic_cast<T>(this->get(name));

  if(value == NULL) {
    // TODO: fnError
    std::cout << "RuntimeError(\"" << *name << " is not defined.\")\n";
    exit(-1);
  }

  return value;
}

void fnBlock::set(std::string* name, fnValue* value) {
  if(this->locals.find(*name) != this->locals.end()) {
    // TODO: fnError
    std::cout << "RuntimeError(\"" << *name << " is already defined.\")\n";
    exit(-1);
  }

  this->locals[*name] = value;
}

std::size_t fnBlock::hash() {
  std::size_t workingHash = (std::size_t)parent;

  for(auto local: locals) {
    std::size_t keyHash = std::hash<std::string>()(local.first);
    std::size_t valueHash = local.second->hash();

    // From http://en.cppreference.com/w/cpp/utility/hash
    workingHash = workingHash ^ (keyHash << 1);
    workingHash = workingHash ^ (valueHash << 1);
  }

  return workingHash;
}

std::string fnBlock::asString(int indentationLevel) {
  std::string result = "{";

  bool putEndingBraceOnNewLine = false;
  for(auto local: locals) {
    result += "\n" + std::string(indentationLevel+INDENT_SIZE, ' ') + local.first + " = " + local.second->asString(indentationLevel + INDENT_SIZE);
    putEndingBraceOnNewLine = true;
  }

  if(putEndingBraceOnNewLine) { result += "\n" + std::string(indentationLevel, ' '); }
  result += "}";
  return result;
}
