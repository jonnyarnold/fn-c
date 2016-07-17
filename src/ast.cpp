#include <iostream>
#include <algorithm>

#include "src/ast.h"
#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

fnValue* astId::execute(fnMachine* context) {
  return context->getValueByName(this->name);
}

fnValue* astDeref::execute(fnMachine* context) {
  fnValue* parentValue = dynamic_cast<fnValue*>(this->parent->execute(context));

  context->pushScopeByValue(parentValue);
  fnValue* value = this->child->execute(context);
  context->popScope();

  return value;
}

fnValue* astBlock::execute(fnMachine* context) {
  fnValue* block = context->pushNewScope();

  fnValue* value = this->executeStatements(context);

  context->popScope();

  return value;
}

fnValue* astBlock::executeStatements(fnMachine* context) {
  // By default, we set the return value to the current block.
  // This ensures that we return the current block if there
  // are no statements in the block.
  fnValue* lastValue = context->currentScope();
  
  for(auto statement: statements) {
    lastValue = statement->execute(context);
  }

  return lastValue;
}

fnValue* astAssignment::execute(fnMachine* context) {
  fnValue* computedValue;

  // TODO: How can we avoid type checking here?
  astDeref* ref = dynamic_cast<astDeref*>(this->key);
  if(ref != NULL) {
    fnValue* parentValue = ref->parent->execute(context);
    context->pushScopeByValue(parentValue);
    computedValue = (new astAssignment((astReference*)ref->child, this->value))->execute(context);
    context->popScope();
  } else {

    // If we don't have an astDeref, we have an astId.
    astId* id = dynamic_cast<astId*>(this->key);
    computedValue = this->value->execute(context);
    context->setValue(id->name, computedValue);

  }

  // Assignments return the block they have been assigned to.
  return context->currentScope();
}

fnValue* astInt::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnNumber(this->value));
}

fnValue* astDouble::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnNumber(this->value));
}

fnValue* astString::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnString(this->value));
}

fnValue* astBool::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnBool(this->value));
}

fnValue* astFnCall::execute(fnMachine* context) {
  // Execute the arguments...
  std::vector<fnValue*>* executedArgs = new std::vector<fnValue*>();
  for(auto arg: (*this->args)) {
    executedArgs->push_back(arg->execute(context));
  }

  fnValue* def = this->target->execute(context);

  return context->callByValue(def, *executedArgs);
}

fnValue* astFnDef::execute(fnMachine* context) {
  return new fnFnDef(this->body, context->currentScope(), this->params);
}

fnValue* astConditional::execute(fnMachine* context) {
  
  fnValue* returnValue;
  for(auto condition: (*this->conditions)) {
    returnValue = condition->execute(context);
    if (returnValue != NULL) { break; }
  }

  // The conditional will return false if
  // no conditions match.
  if (returnValue == NULL) { return new fnBool(false); }
  return returnValue;
}

fnValue* astCondition::execute(fnMachine* context) {
  
  fnValue* testResult = this->test->execute(context);
  if(testResult->asBool()) {
    return this->body->execute(context);
  }

  // NULL shows that the condition failed.
  return NULL;
}
