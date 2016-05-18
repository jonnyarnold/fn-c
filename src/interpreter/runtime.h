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

  virtual std::size_t hash() {
    return (std::size_t)this->value;
  }

  virtual std::string asString(int indentationLevel) = 0;
};

// Represents a bool.
class fnBool : public fnConstant<bool> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) {
    return this->value ? "true" : "false";
  }
};

// Represents an int.
class fnInt : public fnConstant<int> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) {
    return std::to_string(this->value);
  }
};

// Represents a double.
class fnDouble : public fnConstant<double> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) {
    return std::to_string(this->value);
  }
};

// Represents a string.
class fnString : public fnConstant<std::string*> {
  using fnConstant::fnConstant;

public:
  virtual std::size_t hash() {
    return std::hash<std::string>()(*this->value);
  }

  virtual std::string asString(int indentationLevel) {
    return *this->value;
  }
};

// Represents a function definition.
// (FORWARD DECLARATION)
class fnDef;

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
  fnDef* getDefById(std::string* id);

  virtual std::size_t hash() {
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

  virtual std::string asString(int indentationLevel) {
    std::string result = "{\n";
    for(auto local: locals) {
      result += std::string(indentationLevel+INDENT_SIZE, ' ') + local.first + " = " + local.second->asString(indentationLevel + INDENT_SIZE) + "\n";
    }

    result += std::string(indentationLevel, ' ') + "}";
    return result;
  }
};

// Represents a function definition.
class fnDef : public fnValue {
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

  fnValue* call(fnMachine*, std::vector<fnValue*>);

  virtual std::size_t hash() {
    return (std::size_t)(this);
  }

  virtual std::string asString(int indentationLevel) {
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
