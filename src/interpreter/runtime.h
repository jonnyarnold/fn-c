#ifndef FN_RUNTIME
#define FN_RUNTIME

#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include <functional>

#include "ast.h"

// Base class for all Fn values.
class fnValue {};

// A class holding contextual information about an execution.
// (FORWARD DECLARATION)
class fnExecution;

// The style of function used by Fn.
// typedef fnValue* (*fnFunc)(std::vector<fnValue*>);
typedef std::function<fnValue*(fnExecution*, std::vector<fnValue*>)> fnFunc;

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
  void dump() { std::cout << (*this->value); }
};

// Represents a bool.
class fnBool : public fnConstant<bool> { using fnConstant::fnConstant; };

// Represents an int.
class fnInt : public fnConstant<int> { using fnConstant::fnConstant; };

// Represents a double.
class fnDouble : public fnConstant<double> { using fnConstant::fnConstant; };

// Represents a string.
class fnString : public fnConstant<std::string*> { using fnConstant::fnConstant; };

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
    this->func = [block](fnExecution* context, std::vector<fnValue*> values) { return block->execute(context); };
  }

  fnValue* call(fnExecution*, std::vector<fnValue*>);
};

// A class holding contextual information about an execution.
class fnExecution {
public:
  std::stack<fnBlock*>* blockStack;
  fnExecution() { this->blockStack = new std::stack<fnBlock*>(); }

  fnBlock* currentBlock() { 
    if(this->blockStack->empty()) { return NULL; }
    return this->blockStack->top(); 
  }
};

#endif
