#include <iostream>

#include "ast.h"
#include "interpreter/runtime.h"

fnValue* astId::execute(fnExecution* context) {
  fnValue* value;

  if(this->child != NULL) {
    
    // If the identifier has a child,
    // we need to run in the block of the child.
    std::cout << "PUSH_BLOCK(parent " << (*this->name) << ")\n";
    fnBlock* block = context->currentBlock()->getBlockById(this->name);
    context->blockStack->push(block);

    value = this->child->execute(context);

    std::cout << "POP_BLOCK\n";
    context->blockStack->pop();

  } else {
    std::cout << "GET_VALUE(" << (*this->name) << ")";
    value = context->currentBlock()->get(this->name);
    std::cout << " = " << value << "\n";
  }

  return value;
}

fnValue* astBlock::execute(fnExecution* context) {
  std::cout << "PUSH_NEW_BLOCK(parent " << context->currentBlock() << ")";

  fnBlock* block = new fnBlock(context->currentBlock());
  context->blockStack->push(block);
  std::cout << " = " << block << "\n";

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
    std::cout << "POP_BLOCK_AS_VALUE(" << block << ")\n";
    return block;
  } else {
    std::cout << "RETURN_VALUE(" << lastValue << ")\n";
    return lastValue;
  }
}

fnValue* astAssignment::execute(fnExecution* context) {
  fnValue* computedValue;

  if(this->key->child != NULL) {
    
    // If the identifier has a child,
    // we need to run in the block of the child.
    std::cout << "PUSH_BLOCK(" << (*this->key->name) << ")\n";
    fnBlock* block = context->currentBlock()->getBlockById(this->key->name);
    context->blockStack->push(block);
    computedValue = this->key->child->execute(context);
    
    std::cout << "POP_BLOCK(" << context->currentBlock() << ")\n";
    context->blockStack->pop();

  } else {
    
    // Otherwise, get the value and assign it.
    computedValue = this->value->execute(context);
    
    std::cout << "SET_VALUE(name " << (*this->key->name) << ", value " << computedValue << ")\n";
    context->currentBlock()->set(this->key->name, computedValue);
  }

  // Assignments do not return a value.
  // There's a NULL check in astBlock::execute,
  // to decide whether to return a value or a block.
  return NULL;
}

fnValue* astInt::execute(fnExecution* context) {
  std::cout << "CONST_INT(" << this->value << ")";
  fnValue* value = dynamic_cast<fnValue*>(new fnInt(this->value));
  std::cout << " = " << value << "\n";
  return value;
}

fnValue* astDouble::execute(fnExecution* context) {
  std::cout << "CONST_DOUBLE(" << this->value << "\n";
  fnValue* value = dynamic_cast<fnValue*>(new fnDouble(this->value));
  std::cout << " = " << value << "\n";
  return value;
}

fnValue* astString::execute(fnExecution* context) {
  std::cout << "CONST_STRING(" << (*this->value) << ")\n";
  fnValue* value = dynamic_cast<fnValue*>(new fnString(this->value));
  std::cout << " = " << value << "\n";
  return value;
}

fnValue* astBool::execute(fnExecution* context) {
  std::cout << "CONST_BOOL(" << this->value << ")\n";
  fnValue* value = dynamic_cast<fnValue*>(new fnBool(this->value));
  std::cout << " = " << value << "\n";
  return value;
}

fnValue* astFnCall::execute(fnExecution* context) {
  fnValue* value;

  if(this->name->child != NULL) {
    
    // If the identifier has a child,
    // we need to run in the block of the child.
    std::cout << "PUSH_BLOCK(" << (*this->name->name) << ")";

    fnBlock* block = context->currentBlock()->getBlockById(this->name->name);
    context->blockStack->push(block);

    std::cout << " = " << block << "\n";

    value = this->name->child->execute(context);

    std::cout << "POP_BLOCK(" << context->currentBlock() << ")\n";
    context->blockStack->pop();

  } else {
    // Execute the arguments...
    std::vector<fnValue*>* executedArgs = new std::vector<fnValue*>();
    for(auto arg: (*this->args)) {
      executedArgs->push_back(arg->execute(context));
    }

    std::cout << "CALL(" << (*this->name->name) << ")\n";

    // Get the definition...
    fnDef* def = context->currentBlock()->getDefById(this->name->name);

    value = def->call(context, *executedArgs);
  }

  return value;
}

fnValue* astFnDef::execute(fnExecution* context) {
  std::cout << "CONST_FN(" << this->body << ")";

  fnDef* def = new fnDef(this->body, context->currentBlock(), this->params);
  std::cout << " = " << def << "\n";
  return def;
}

fnValue* astCondition::execute(fnExecution* context) {
  return NULL;
}

fnValue* astConditional::execute(fnExecution* context) {
  return NULL;
}

