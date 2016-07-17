#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// A few forward declarations...
class fnValue;
class fnDef;
class fnMachine;

// Shorthand for a dictionary of values.
typedef std::unordered_map<std::string, fnValue*> ValueDict;

// Base class for all Fn values.
class fnValue {
protected:

  // Stores attributes that are nested within the value.
  ValueDict locals;

  // Stores builtin attributes that can be overwritten.
  ValueDict builtins;


  // Super-special constructor.
  // This is used for fnDefs attached to fnValue,
  // otherwise an infinite loop occurs because
  // fnDef's constructor calls fnValue's constructor,
  // which calls a new fnDef...
  //
  // So this is bare. It has no locals or parent.
  fnValue() {
    this->parent = NULL;
    this->locals = ValueDict{};
    this->builtins = ValueDict{};
  }

public:

  // If a value has a parent, the parent will be searched for a value
  // if this value does not have it in its locals.
  fnValue* parent;

  fnValue(fnValue* parent);

  // Return a value for comparison.
  virtual std::size_t hash();

  // Returns a string representing the value.
  virtual std::string asString(int indentationLevel = 0);

  // In fn, all values (except for false) are true.
  virtual bool asBool() { return true; }

  // Get a local value.
  fnValue* get(std::string* name);

  // Set a value to the locals of the current value.
  void set(std::string* name, fnValue* value);

  // Called when the value is invoked like a function.
  //
  // By default, call() will get the value specified in the
  // first argument.
  virtual fnValue* call(fnMachine* context, std::vector<fnValue*> values) {
    std::string name = values[0]->asString();
    return this->get(&name);
  };

};
