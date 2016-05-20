#include <iostream>

#include "src/interpreter/machine.h"

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

fnBlock* fnMachine::pushNewBlock(fnBlock* parent) {
  fnBlock* block = new fnBlock(this->currentBlock());
  this->blockStack->push(block);

  DEBUG("PUSH_NEW_BLOCK(parent: " << parent << ") = " << block);

  return block;
}

void fnMachine::pushBlockByName(std::string* name) {
  fnBlock* block = this->currentBlock()->getBlockById(name);
  this->blockStack->push(block);

  DEBUG("PUSH_BLOCK(name: " << *name << ")");
}

fnBlock* fnMachine::currentBlock() {
  fnBlock* block;

  if(this->blockStack->empty()) { block = NULL; }
  else { block = this->blockStack->top(); }

  // This isn't really an operation. Should we output it?
  // DEBUG("CURRENT_BLOCK() = " << block);

  return block;
}

void fnMachine::popBlock() {
  this->blockStack->pop();
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

fnValue* fnMachine::callByName(std::string* name, std::vector<fnValue*> args) {
  fnCallable* def = this->currentBlock()->getDefById(name);
  fnValue* value = def->call(this, args);

  DEBUG("CALL(name: " << *name << ", args: ...) = " << value->asString());

  return value;
}
