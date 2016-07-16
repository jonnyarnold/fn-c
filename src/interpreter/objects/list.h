#pragma once

#include "src/interpreter/machine.h"
#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/number.h"

// Number of spaces between indent levels when converting to string.
const int INDENT_SIZE = 2;

fnValue* map(fnMachine* context, std::vector<fnValue*> values);

// Represents an array of values.
class fnList : public fnValue {
public:
  std::vector<fnValue*> values;

  fnList(std::vector<fnValue*> values) : fnValue(new fnWorld()) {
    this->values = values;

    this->locals = ValueDict{
      {"map", new fnDef(&map, this, new Strings{"func"})},
    };
  }

  virtual std::size_t hash() override {
    std::size_t workingHash;

    for(auto value: values) {
      std::size_t valueHash = value->hash();

      // From http://en.cppreference.com/w/cpp/utility/hash
      workingHash = workingHash ^ (valueHash << 1);
    }

    return workingHash;
  }

  // The default call to fnList returns
  // the element at the given index.
  fnValue* call(fnMachine* context, std::vector<fnValue*> values) override {
    int index = static_cast<fnInt*>(values[0])->value;

    return this->values[index];
  }

  virtual std::string asString(int indentationLevel) override {
    std::string result = "List(";
    for(auto value: this->values) {
      result += value->asString();
      if(value != this->values.back()) {
        result += ", ";
      }
    }

    result += ")";

    return result;
  }
};
