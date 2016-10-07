#pragma once

#include "src/interpreter/objects/constant.h"

// Represents a string.
class fnString : public fnConstant<std::string*> {

public:
  fnString(std::string*);

  virtual std::size_t hash() override {
    return std::hash<std::string>()(*this->value);
  }

  virtual std::string asString(int indentationLevel) override {
    return *this->value;
  }
};
