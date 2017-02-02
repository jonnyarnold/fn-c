#include <iostream>

#include "src/interpreter/machine.h"

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

fnValue* fnMachine::pushNewScope(fnValue* parent) {
  fnValue* scope = new fnValue(parent);

  this->scopeStack->push(scope);

  DEBUG("PUSH_NEW_SCOPE(parent: " << parent << ") = " << scope);

  return scope;
}

void fnMachine::pushScopeByValue(fnValue* scope) {
  this->scopeStack->push(scope);

  DEBUG("PUSH_SCOPE(" << scope->asString() << ")");
}

fnValue* fnMachine::currentScope() {

  // This guard should never happen.
  // We always have a fnWorld on the stack to start with,
  // and all other operations push and pop their own blocks
  // onto the top of the stack.
  if(this->scopeStack->empty()) { 
    std::cout << "!!! Scope stack is empty!";
    return NULL;
  }

  return this->scopeStack->top();
}

void fnMachine::popScope() {
  fnValue* poppedScope = this->scopeStack->top();
  this->scopeStack->pop();
  
  DEBUG("POP_SCOPE(" << poppedScope->asString(0) << ")");
}

void fnMachine::setValue(std::string* name, fnValue* value) {
  this->currentScope()->set(name, value);
  DEBUG("SET_VALUE(name: " << *name << ", value: " << value->asString() << ")");
}

fnValue* fnMachine::getValueByName(std::string* name) {
  fnValue* value = this->currentScope()->get(name);

  DEBUG("GET_VALUE(name: " << *name << ") = " << value->asString());

  return value;
}

fnValue* fnMachine::callByValue(fnValue* def, std::vector<fnValue*> args) {
  DEBUG("START_CALL(" << def->asString(0) << ")")
  fnValue* value = def->call(this, args);
  DEBUG("END_CALL(" << def->asString(0) << ")")

  return value;
}
