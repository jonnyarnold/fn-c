#pragma once

#include "src/interpreter/objects/constant.h"

// Represents a bool.
class fnBool : public fnConstant<bool> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) override {
    return this->value ? "true" : "false";
  }

  virtual bool asBool() override { return this->value; }
};
