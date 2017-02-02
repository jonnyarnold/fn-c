#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/defs/eq.h"
#include "src/interpreter/objects/defs/and.h"
#include "src/interpreter/objects/defs/or.h"
#include "src/interpreter/objects/defs/include.h"
#include "src/interpreter/objects/defs/load.h"
#include "src/interpreter/machine.h"
#include "src/errors.h"

#include <iostream>

// Number of spaces between indent levels when converting to string.
const int INDENT_SIZE = 2;

fnValue::fnValue(fnValue* parent) {
  this->parent = parent;

  this->builtins = ValueDict{
    {"eq", new fnEq(this)},
    {"and", new fnAnd(this)},
    {"or", new fnOr(this)},
    {"include", new fnInclude(this)},
    {"load", new fnLoad()},
  };

  this->locals = ValueDict{};
}

fnValue::~fnValue() {
  // Don't delete the parent!
  this->locals.clear();
  this->builtins.clear();
}

fnValue* fnValue::get(std::string* name) {
  fnValue* value = this->locals[(*name)];

  if (value == NULL) {
    // Try the builtins
    value = this->builtins[(*name)];
  }

  if (value == NULL) {
    if(this->parent != NULL) { return this->parent->get(name); }
    else {
      throw FnRuntimeError(*name + " is not defined in " + this->asString(0));
    }
  }

  return value;
}

void fnValue::set(std::string* name, fnValue* value) {
  if(this->locals.find(*name) != this->locals.end()) {
    throw FnRuntimeError(*name + " is already defined in " + this->asString(0));
  }

  this->locals[*name] = value;
}

std::size_t fnValue::hash() {
  std::size_t workingHash = (std::size_t)parent;

  for(auto local: locals) {   
    std::size_t keyHash = std::hash<std::string>()(local.first);

    // TODO: Why is this happening?!
    if (local.second == NULL) {
      std::cout << "WARNING: Undefined attribute " << local.first << " in " << this->asString(0) << std::endl;
      continue;
    }

    std::size_t valueHash = local.second->hash();

    // From http://en.cppreference.com/w/cpp/utility/hash
    workingHash = workingHash ^ (keyHash << 1);
    workingHash = workingHash ^ (valueHash << 1);
  }

  return workingHash;
}

std::string fnValue::asString(int indentationLevel) {
  std::string result = "{";

  bool endBraceOnNewLine = false;
  for(auto local: locals) {
    if(local.second != NULL) {
      result += "\n" + std::string(indentationLevel+INDENT_SIZE, ' ') + local.first + " = " + local.second->asString(indentationLevel + INDENT_SIZE);
      endBraceOnNewLine = true;
    }  
  }

  if (endBraceOnNewLine) { result += "\n" + std::string(indentationLevel, ' '); }
  result += "}";
  return result;
}
