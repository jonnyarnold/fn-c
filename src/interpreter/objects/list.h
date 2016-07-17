#pragma once

#include "src/interpreter/machine.h"
#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/number.h"

// Number of spaces between indent levels when converting to string.
const int INDENT_SIZE = 2;

class fnList;

class fnMap : public fnDef {
  fnList* list;
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values);

public:
  fnMap(fnList* parentList) : fnDef(parent, new std::vector<std::string>{"fn"}) {
    this->list = parentList;
  }
};

// Represents an array of values.
class fnList : public fnValue {
public:
  std::vector<fnValue*> values;

  fnList(std::vector<fnValue*> values) : fnValue(NULL) {
    this->values = values;

    this->locals = ValueDict{
      {"map", new fnMap(this)},
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
    int index = static_cast<fnNumber*>(values[0])->asInt();
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
