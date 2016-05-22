#pragma once

#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include <functional>

#include "src/ast.h"

const int INDENT_SIZE = 2;

// Base class for all Fn values.
class fnValue {
public:
  // Return a value for comparison.
  virtual std::size_t hash() = 0;

  // Returns a string representing the value.
  virtual std::string asString(int indentationLevel) = 0;
  std::string asString() {
    return this->asString(0);
  };
};

// A class holding contextual information about an execution.
// (FORWARD DECLARATION)
class fnMachine;

// The style of function used by Fn.
// typedef fnValue* (*fnFunc)(std::vector<fnValue*>);
typedef std::function<fnValue*(fnMachine*, std::vector<fnValue*>)> fnFunc;

// A vector of strings.
typedef std::vector<std::string> Strings;

// A dictionary of values.
typedef std::unordered_map<std::string, fnValue*> ValueDict;

// Represents any constant value in Fn.
// The type T is the C-type that the value
// belongs to.
template<class T>
class fnConstant : public fnValue {
public:
  T value;
  fnConstant(T value) { this->value = value; }

  virtual std::size_t hash() override {
    return (std::size_t)this->value;
  }

  virtual std::string asString(int indentationLevel) override = 0;
};

// Represents a bool.
class fnBool : public fnConstant<bool> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) override {
    return this->value ? "true" : "false";
  }
};

// Represents an int.
class fnInt : public fnConstant<int> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) override {
    return std::to_string(this->value);
  }
};

// Represents a double.
class fnDouble : public fnConstant<double> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) override {
    return std::to_string(this->value);
  }
};

// Represents a string.
class fnString : public fnConstant<std::string*> {
  using fnConstant::fnConstant;

public:
  virtual std::size_t hash() override {
    return std::hash<std::string>()(*this->value);
  }

  virtual std::string asString(int indentationLevel) override {
    return *this->value;
  }
};

// Represents a function definition.
// (FORWARD DECLARATION)
class fnCallable;

// At runtime, an aggregate structure
// storing values.
class fnBlock : public fnValue {
protected:
  fnBlock* parent;
  ValueDict locals;

public:
  fnBlock(fnBlock* parent) { this->parent = parent; }
  void set(std::string* name, fnValue* value);
  fnValue* get(std::string* name);
  fnBlock* getBlockById(std::string* id);
  fnCallable* getDefById(std::string* id);

  virtual std::size_t hash() override {
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

  virtual std::string asString(int indentationLevel) override {
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
};

// Abstract class to denote callable objects.
class fnCallable : public fnValue {
public:
  virtual fnValue* call(fnMachine*, std::vector<fnValue*>) = 0;
};

// Represents a function definition.
class fnDef : public fnCallable {
  fnBlock* parentBlock;
  Strings* params;
  fnFunc func;

public:
  fnDef(fnFunc instructions, fnBlock* parentBlock, Strings* params) {
    this->parentBlock = parentBlock;
    this->params = params;
    this->func = instructions;
  }

  fnDef(astBlock* block, fnBlock* parentBlock, Strings* params) {
    this->parentBlock = parentBlock;
    this->params = params;
    this->func = [block](fnMachine* context, std::vector<fnValue*> values) { return block->execute(context); };
  }

  fnValue* call(fnMachine*, std::vector<fnValue*>) override;

  virtual std::size_t hash() override {
    return (std::size_t)(this);
  }

  virtual std::string asString(int indentationLevel) override {
    std::string result = "(";
    for(auto param: (*this->params)) {
      result += param;
      if(param != this->params->back()) {
        result += ", ";
      }
    }

    result += ") { ... }";

    return result;
  }
};

// Represents an array of values.
class fnList : public fnCallable {
  std::vector<fnValue*> values;

public:
  fnList(std::vector<fnValue*> values) {
    this->values = values;
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
