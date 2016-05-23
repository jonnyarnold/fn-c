#pragma once

#include <string>

#include "src/interpreter/objects/value.h"

// At runtime, an aggregate structure
// storing values.
class fnBlock : public fnValue {
protected:
  fnBlock* parent;

public:
  fnBlock(fnBlock* parent) { this->parent = parent; }
  
  // Get a value.
  fnValue* get(std::string* name) override;

  // Get a value, cast to the given type.
  template<typename T>
  T get(std::string* name);

  // Set a value to the locals of the current value.
  void set(std::string* name, fnValue* value);

  virtual std::size_t hash() override;
  virtual std::string asString(int indentationLevel) override;
};
