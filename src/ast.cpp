#include <iostream>
#include <algorithm>

#include "src/ast.h"
#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

fnValue* astId::execute(fnMachine* context) {
  return context->getValueByName(this->name);

  // fnValue* value;

  // if(this->child != NULL) {

  //   // If the identifier has a child,
  //   // we need to run in the block of the child.
  //   context->pushBlockByName(this->name);
  //   value = this->child->execute(context);
  //   context->popBlock();

  // } else {
  //   value = context->getValueByName(this->name);
  // }

  // return value;
}

fnValue* astDeref::execute(fnMachine* context) {
  fnBlock* parentValue = dynamic_cast<fnBlock*>(this->parent->execute(context));
  context->pushBlockByValue(parentValue);
  fnValue* value = this->child->execute(context);
  context->popBlock();

  return value;
}

fnValue* astBlock::execute(fnMachine* context) {
  fnBlock* block = context->pushNewBlock();

  fnValue* value = this->executeStatements(context);

  context->popBlock();

  return value;
}

fnValue* astBlock::executeStatements(fnMachine* context) {
  // By default, we set the return value to the current block.
  // This ensures that we return the current block if there
  // are no statements in the block.
  fnValue* lastValue = context->currentBlock();
  
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
    fnBlock* parentValue = dynamic_cast<fnBlock*>(ref->parent->execute(context));
    context->pushBlockByValue(parentValue);
    computedValue = (new astAssignment((astReference*)ref->child, this->value))->execute(context);
    context->popBlock();
  } else {

    // If we don't have an astDeref, we have an astId.
    astId* id = dynamic_cast<astId*>(this->key);
    computedValue = this->value->execute(context);
    context->setValue(id->name, computedValue);

  }

  // Assignments return the block they have been assigned to.
  return context->currentBlock();
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
  fnDef* def = dynamic_cast<fnDef*>(this->target->execute(context));

  // Execute the arguments...
  std::vector<fnValue*>* executedArgs = new std::vector<fnValue*>();
  for(auto arg: (*this->args)) {
    executedArgs->push_back(arg->execute(context));
  }

  return context->callByValue(def, *executedArgs);
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

