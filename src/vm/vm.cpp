#include "src/vm/vm.h"

#include <iostream>
#include <bitset>

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

fnVM::fnVM(bool debug) {
  this->debug = debug;
  this->values = std::vector<fnVMValue*>();
}

fnVM::~fnVM() {
  this->values.clear();
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

    case FN_OP_AND:
      returnValue = this->fnAnd(&instructions[counter]);
      counter += 3;
      break;

    case FN_OP_OR:
      returnValue = this->fnOr(&instructions[counter]);
      counter += 3;
      break;

    case FN_OP_NOT:
      returnValue = this->fnNot(&instructions[counter]);
      counter += 2;
      break;

    default:
      throw -1; // TODO: Make this more meaningful

    }
  }

  return *returnValue;
}

fnVMValue* fnVM::value(fnByte index) {
  return this->values[index];
}

fnVMValue* fnVM::declareBool(fnByte value) {
  return this->declareBool((bool)value);
}

fnVMValue* fnVM::declareBool(bool value) {
  fnVMValue* b = new fnVMValue;
  b->asBool = value;

  this->values.push_back(b);
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

  this->values.push_back(n);

  return n;
}

fnVMValue* fnVM::fnAnd(fnByte value[]) {
  // AND is of the form:
  // [1-BYTE AND_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // AND will return true if both arguments are true,
  // false otherwise.
  bool first = this->value(value[1])->asBool;
  if (!first) {
    return this->declareBool(FN_OP_FALSE);
  }

  bool second = this->value(value[2])->asBool;
  if (!second) {
    return this->declareBool(FN_OP_FALSE);
  }

  return this->declareBool(FN_OP_TRUE);
}

fnVMValue* fnVM::fnOr(fnByte value[]) {
  // OR is of the form:
  // [1-BYTE OR_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // OR will return true if either argument is true;
  // false otherwise.
  bool first = this->value(value[1])->asBool;
  if (first) {
    return this->declareBool(FN_OP_TRUE);
  }

  bool second = this->value(value[2])->asBool;
  if (second) {
    return this->declareBool(FN_OP_TRUE);
  }

  return this->declareBool(FN_OP_FALSE);
}

fnVMValue* fnVM::fnNot(fnByte value[]) {
  // NOT is of the form:
  // [1-BYTE NOT_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // NOT will return true if the argument is false,
  // and vice versa.
  bool arg = this->value(value[1])->asBool;
  if (arg) {
    return this->declareBool(FN_OP_FALSE);
  } else {
    return this->declareBool(FN_OP_TRUE);
  }
}
