#include "src/vm/vm.h"

#include <bitset>
#include <iostream>

fnVM::fnVM(bool debug) {
  this->debug = debug;
  this->values = std::stack<fnVMValue*>();
}

fnVM::~fnVM() {
  while(!this->values.empty()) {
    fnVMValue* top = this->values.top();
    this->values.pop();
    delete top;
  }
}

fnVMValue fnVM::run(fnByte instructions[], size_t num_bytes) {
  size_t counter = 0;
  fnVMValue* returnValue = NULL;

  while(counter < num_bytes) {
    fnByte opcode = instructions[counter];

    switch(opcode) {

    case FN_OP_FALSE:
    case FN_OP_TRUE:
      returnValue = this->declareBool(opcode);
      counter += 1;
      break;

    case FN_OP_NUMBER:
      returnValue = this->declareNumber(&instructions[counter]);
      counter += 8;
      break;

    default:
      throw -1; // TODO: Make this more meaningful

    }
  }

  return *returnValue;
}

fnVMValue* fnVM::declareBool(fnByte value) {
  fnVMValue* b = new fnVMValue;
  b->asBool = (bool)value;

  this->values.push(b);

  return b;
}

fnVMValue* fnVM::declareNumber(fnByte value[]) {
  // A Number is of the form:
  // [1-BYTE NUMBER_OP_CODE] [1-BYTE EXPONENT] [6-BYTE COEFFICIENT]
  // (8 bytes total)
  fnExponent exponent = (fnExponent)value[1];
  fnCoefficient coefficient = (fnCoefficient)value[2];

  fnVMValue* n = new fnVMValue;
  n->asNumber = fnVMNumber(exponent, coefficient);

  this->values.push(n);

  return n;
}
