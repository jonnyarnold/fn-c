#include <string>
#include <unordered_map>
#include <stack>
#include <vector>

#ifndef FN_RUNTIME
#define FN_RUNTIME

// Base class for all Fn values.
class fnValue {};

// The style of function used by Fn.
typedef fnValue (*fnFunc)(std::vector<fnValue>);

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
class fnDef : public fnConstant<fnFunc> { using fnConstant::fnConstant; };

// At runtime, an aggregate structure
// storing values.
class fnBlock : public fnValue {
  fnBlock* parent;
  std::unordered_map<std::string, fnValue*> locals;

public:
  fnBlock(fnBlock* parent) { this->parent = parent; }
  void set(std::string* name, fnValue* value);
  fnValue* get(std::string* name);
  fnBlock* getBlockById(std::string* id);
};

// A class holding contextual information
// about an execution.
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
