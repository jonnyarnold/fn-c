#ifndef FN_MACHINE
#define FN_MACHINE

#include <stack>

#include "interpreter/runtime.h"

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
