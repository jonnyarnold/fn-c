#include <iostream>

#include "src/interpreter/machine.h"

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

fnBlock* fnMachine::pushNewBlock(fnBlock* parent) {
  fnBlock* block = new fnBlock(parent);
  this->blockStack->push(block);

  DEBUG("PUSH_NEW_BLOCK(parent: " << parent << ") = " << block);

  return block;
}

void fnMachine::pushBlockByValue(fnBlock* block) {
  this->blockStack->push(block);

  DEBUG("PUSH_BLOCK(" << block << ")");
}

fnBlock* fnMachine::currentBlock() {

  // This guard should never happen.
  // We always have a fnTopBlock on the stack to start with,
  // and all other operations push and pop their own blocks
  // onto the top of the stack.
  if(this->blockStack->empty()) { 
    std::cout << "!!! Block stack is empty!";
    return NULL;
  }

  return this->blockStack->top();
}

void fnMachine::popBlock() {
  this->blockStack->pop();

  // TODO: Garbage collection?

  DEBUG("POP_BLOCK()");
}

void fnMachine::setValue(std::string* name, fnValue* value) {
  this->currentBlock()->set(name, value);
  DEBUG("SET_VALUE(name: " << *name << ", value: " << value->asString() << ")");
}

fnValue* fnMachine::getValueByName(std::string* name) {
  fnValue* value = this->currentBlock()->get(name);

  DEBUG("GET_VALUE(name: " << *name << ") = " << value->asString());

  return value;
}

fnValue* fnMachine::callByValue(fnDef* def, std::vector<fnValue*> args) {
  fnValue* value = def->call(this, args);

  DEBUG("CALL(def: " << def << ", args: ...) = " << value->asString());

  return value;
}
