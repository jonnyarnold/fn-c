#pragma once

#include <iostream>
#include <stack>

#include "src/interpreter/world.h"
#include "src/interpreter/objects/value.h"

// A class holding contextual information about an execution.
class fnMachine {
public:
  std::stack<fnValue*>* scopeStack;
  bool debug;

  fnMachine(bool debug) {
    this->debug = debug;
    this->scopeStack = new std::stack<fnValue*>();
    this->scopeStack->push(new fnWorld());

    if(this->debug) { std::cout << "CREATE_MACHINE()" << std::endl; }
  }

  fnMachine() : fnMachine(false) {}



  // ===
  // BLOCKS
  // ===

  // Creates a new block on the top of the block stack.
  fnValue* pushNewScope(fnValue*);
  fnValue* pushNewScope() { return this->pushNewScope(this->currentScope()); }

  // Gets a value by name in the current block
  // and pushes it onto the top of the block stack.
  void pushScopeByValue(fnValue*);

  // Gets the block at the top of the block stack.
  fnValue* currentScope();

  // Get the top of the scope stack as a given type.
  template<typename T>
  T currentScope();

  // Removes the block at the top of the block stack.
  void popScope();



  // ===
  // VALUES
  // ===

  // Sets a value in the current block.
  void setValue(std::string* name, fnValue* value);

  // Gets a value from the current block by name.
  fnValue* getValueByName(std::string*);

  // Gets a value from the current block
  // and calls it with the given arguments.
  fnValue* callByValue(fnValue* def, std::vector<fnValue*> args);

  void printState() {
    std::stack<fnValue*> printStack = *this->scopeStack;

    std::cout << "MACHINE_STACK = List(\n";

    while(!printStack.empty()) {
      fnValue* scope = printStack.top();

      std::cout << "  " << scope->asString(2) << ",\n";
      
      printStack.pop();
    }

    std::cout << ")\n";
  }
};
