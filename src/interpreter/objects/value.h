#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// A few forward declarations...
class fnValue;
class fnMachine;

// Shorthand for a dictionary of values.
typedef std::unordered_map<std::string, fnValue*> ValueDict;

// Base class for all Fn values.
class fnValue {
protected:

  // Stores attributes that are nested within the value.
  ValueDict locals;

public:

  fnValue() {
    this->locals = ValueDict{};
  }

  // Return a value for comparison.
  virtual std::size_t hash() = 0;

  // Returns a string representing the value.
  virtual std::string asString(int indentationLevel = 0) = 0;

  // In fn, all values (except for false) are true.
  virtual bool asBool() { return true; }

  // Get a local value.
  virtual fnValue* get(std::string* name);

  // Called when the value is invoked like a function.
  //
  // By default, call() will get the value specified in the
  // first argument.
  virtual fnValue* call(fnMachine* context, std::vector<fnValue*> values) {
    std::string name = values[0]->asString();
    return this->get(&name);
  };

};
