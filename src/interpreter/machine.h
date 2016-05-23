#pragma once

#include <stack>

#include "src/interpreter/builtins.h"
#include "src/interpreter/runtime.h"

// A class holding contextual information about an execution.
class fnMachine {
public:
  std::stack<fnBlock*>* blockStack;
  bool debug;

  fnMachine(bool debug) {
    this->debug = debug;
    this->blockStack = new std::stack<fnBlock*>();
    fnBlock* topBlock = new fnTopBlock();
    this->blockStack->push(topBlock);

    if(this->debug) { std::cout << "CREATE_MACHINE() = " << topBlock << std::endl; }
  }

  fnMachine() : fnMachine(false) {}



  // ===
  // BLOCKS
  // ===

  // Creates a new block on the top of the block stack.
  fnBlock* pushNewBlock(fnBlock* parent);
  fnBlock* pushNewBlock() { return this->pushNewBlock(this->currentBlock()); }

  // Gets a value by name in the current block
  // and pushes it onto the top of the block stack.
  void pushBlockByValue(fnBlock*);

  // Gets the block at the top of the block stack.
  fnBlock* currentBlock();

  // Removes the block at the top of the block stack.
  void popBlock();



  // ===
  // VALUES
  // ===

  // Sets a value in the current block.
  void setValue(std::string* name, fnValue* value);

  // Gets a value from the current block by name.
  fnValue* getValueByName(std::string*);

  // Gets a value from the current block
  // and calls it with the given arguments.
  fnValue* callByValue(fnDef* def, std::vector<fnValue*> args);

};
