#include <iostream>
#include <algorithm>

#include "src/ast.h"
#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

fnValue* astId::execute(fnMachine* context) {
  fnValue* value;

  if(this->child != NULL) {

    // If the identifier has a child,
    // we need to run in the block of the child.
    context->pushBlockByName(this->name);
    value = this->child->execute(context);
    context->popBlock();

  } else {
    value = context->getValueByName(this->name);
  }

  return value;
}

fnValue* astBlock::execute(fnMachine* context) {
  fnBlock* block = context->pushNewBlock();

  fnValue* value = this->executeStatements(context);

  context->popBlock();

  return value;
}

fnValue* astBlock::executeStatements(fnMachine* context) {
  fnValue* lastValue;
  for(auto statement: statements) {
    lastValue = statement->execute(context);
  }

  // If we have an assignment as the last statement,
  // return the block...
  if(lastValue == NULL) {
    return context->currentBlock();
  } else {
    return lastValue;
  }
}

fnValue* astAssignment::execute(fnMachine* context) {
  fnValue* computedValue;

  if(this->key->child != NULL) {

    // If the identifier has a child,
    // we need to run in the block of the child.
    context->pushBlockByName(this->key->name);
    computedValue = this->key->child->execute(context);
    context->popBlock();

  } else {

    // Otherwise, get the value and assign it.
    computedValue = this->value->execute(context);
    context->setValue(this->key->name, computedValue);
  }

  // Assignments do not return a value.
  // There's a NULL check in astBlock::execute,
  // to decide whether to return a value or a block.
  return NULL;
}

fnValue* astInt::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnInt(this->value));
}

fnValue* astDouble::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnDouble(this->value));
}

fnValue* astString::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnString(this->value));
}

fnValue* astBool::execute(fnMachine* context) {
  return dynamic_cast<fnValue*>(new fnBool(this->value));
}

fnValue* astFnCall::execute(fnMachine* context) {
  fnValue* value;

  if(this->name->child != NULL) {

    // If the identifier has a child,
    // we need to run in the block of the child.
    context->pushBlockByName(this->name->name);
    value = (new astFnCall(this->name->child, this->args))->execute(context);
    context->popBlock();

  } else {

    // Execute the arguments...
    std::vector<fnValue*>* executedArgs = new std::vector<fnValue*>();
    for(auto arg: (*this->args)) {
      executedArgs->push_back(arg->execute(context));
    }

    value = context->callByName(this->name->name, *executedArgs);
  }

  return value;
}

fnValue* astFnDef::execute(fnMachine* context) {
  return new fnDef(this->body, context->currentBlock(), this->params);
}

fnValue* astCondition::execute(fnMachine* context) {
  return NULL;
}

fnValue* astConditional::execute(fnMachine* context) {
  return NULL;
}

