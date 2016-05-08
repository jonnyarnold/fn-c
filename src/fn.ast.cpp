#include <iostream>

#include "fn.ast.h"
#include "fn.runtime.h"

fnValue* astId::execute(fnExecution* context) {
  fnValue* value;

  if(this->child != NULL) {
    
    // If the identifier has a child,
    // we need to run in the block of the child.
    std::cout << "PUSH_BLOCK " << (*this->name) << "\n";

    fnBlock* block = context->currentBlock()->getBlockById(this->name);
    context->blockStack->push(block);

    value = this->child->execute(context);

    std::cout << "POP_BLOCK\n";
    context->blockStack->pop();

  } else {
    std::cout << "GET " << (*this->name) << "\n";
    value = context->currentBlock()->get(this->name);
  }

  return value;
}

fnValue* astBlock::execute(fnExecution* context) {
  std::cout << "PUSH_NEW_BLOCK\n";

  fnBlock* block = new fnBlock(context->currentBlock());
  context->blockStack->push(block);

  // Statements are added to blocks in reverse order
  // by Flex/Bison. 
  fnValue* lastValue; 
  std::reverse(statements.begin(), statements.end());
  for(auto statement: statements) {
    lastValue = statement->execute(context);
  }

  context->blockStack->pop();

  // If we have an assignment as the last statement,
  // return the block...

  if(lastValue == NULL) {
    std::cout << "POP_BLOCK_AS_VALUE\n";
    return block;
  } else {
    std::cout << "RETURN\n";
    return lastValue;
  }
}

fnValue* astAssignment::execute(fnExecution* context) {
  fnValue* computedValue;

  if(this->key->child != NULL) {
    
    // If the identifier has a child,
    // we need to run in the block of the child.
    std::cout << "PUSH_BLOCK " << (*this->key->name) << "\n";
    fnBlock* block = context->currentBlock()->getBlockById(this->key->name);
    context->blockStack->push(block);
    computedValue = this->key->child->execute(context);
    
    std::cout << "POP_BLOCK\n";
    context->blockStack->pop();

  } else {
    
    // Otherwise, get the value and assign it.
    computedValue = this->value->execute(context);
    
    std::cout << "SET " << (*this->key->name) << "\n";
    context->currentBlock()->set(this->key->name, computedValue);
  }

  return NULL;
}

fnValue* astInt::execute(fnExecution* context) {
  std::cout << "INT " << this->value << "\n";
  return dynamic_cast<fnValue*>(new fnInt(this->value));
}

fnValue* astDouble::execute(fnExecution* context) {
  std::cout << "DOUBLE " << this->value << "\n";
  return dynamic_cast<fnValue*>(new fnDouble(this->value));
}

fnValue* astString::execute(fnExecution* context) {
    std::cout << "STRING " << (*this->value) << "\n";
  return dynamic_cast<fnValue*>(new fnString(this->value));
}

fnValue* astBool::execute(fnExecution* context) {
    std::cout << "BOOL " << this->value << "\n";
  return dynamic_cast<fnValue*>(new fnBool(this->value));
}

fnValue* astFnCall::execute(fnExecution* context) {
  return NULL;
}

fnValue* astFnDef::execute(fnExecution* context) {
  return NULL;
}

fnValue* astCondition::execute(fnExecution* context) {
  return NULL;
}

fnValue* astConditional::execute(fnExecution* context) {
  return NULL;
}

