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

    case FN_OP_MULTIPLY:
      returnValue = this->fnMultiply(&instructions[counter]);
      counter += 3;
      break;

    case FN_OP_DIVIDE:
      returnValue = this->fnDivide(&instructions[counter]);
      counter += 3;
      break;

    case FN_OP_ADD:
      returnValue = this->fnAdd(&instructions[counter]);
      counter += 3;
      break;

    case FN_OP_SUBTRACT:
      returnValue = this->fnSubtract(&instructions[counter]);
      counter += 3;
      break;

    default:
      throw "Unexpected opcode"; // TODO: Make this more meaningful

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
  DEBUG("DECLARE_BOOL(" << value << ")");
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

  DEBUG("DECLARE_NUMBER(" << (int)coefficient << " * 10^" << (int)exponent << ")");

  fnVMValue* n = new fnVMValue;
  n->asNumber = fnVMNumber(exponent, coefficient);

  this->values.push_back(n);

  return n;
}

fnVMValue* fnVM::declareNumber(fnVMNumber value) {
  DEBUG("DECLARE_NUMBER(" << (int)value.coefficient << " * 10^" << (int)value.exponent << ")");

  fnVMValue* n = new fnVMValue;
  n->asNumber = value;

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
    DEBUG("AND(false, ???)");
    return this->declareBool(FN_OP_FALSE);
  }

  bool second = this->value(value[2])->asBool;
  if (!second) {
    DEBUG("AND(true, false)");
    return this->declareBool(FN_OP_FALSE);
  }

  DEBUG("AND(true, true)");
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
    DEBUG("OR(true, ???)");
    return this->declareBool(FN_OP_TRUE);
  }

  bool second = this->value(value[2])->asBool;
  if (second) {
    DEBUG("OR(false, true)");
    return this->declareBool(FN_OP_TRUE);
  }

  DEBUG("OR(false, false)");
  return this->declareBool(FN_OP_FALSE);
}

fnVMValue* fnVM::fnNot(fnByte value[]) {
  // NOT is of the form:
  // [1-BYTE NOT_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // NOT will return true if the argument is false,
  // and vice versa.
  bool arg = this->value(value[1])->asBool;
  DEBUG("NOT(" << arg << ")");

  if (arg) {
    return this->declareBool(FN_OP_FALSE);
  } else {
    return this->declareBool(FN_OP_TRUE);
  }
}

fnVMValue* fnVM::fnMultiply(fnByte value[]) {
  // MULTIPLY is of the form:
  // [1-BYTE MULTIPLY_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // MULTIPLY will return the product of the given values.
  fnVMNumber first = this->value(value[1])->asNumber;
  fnVMNumber second = this->value(value[2])->asNumber;

  DEBUG("MULTIPLY(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  fnVMNumber product = first * second;

  return this->declareNumber(product);
}

fnVMValue* fnVM::fnDivide(fnByte value[]) {
  // DIVIDE is of the form:
  // [1-BYTE DIVIDE_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // DIVIDE will return the fraction (first / second) as a Number.
  fnVMNumber first = this->value(value[1])->asNumber;
  fnVMNumber second = this->value(value[2])->asNumber;

  DEBUG("DIVIDE(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  fnVMNumber fraction = first / second;

  return this->declareNumber(fraction);
}

fnVMValue* fnVM::fnAdd(fnByte value[]) {
  // ADD is of the form:
  // [1-BYTE ADD_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // ADD will return the sum as a Number.
  fnVMNumber first = this->value(value[1])->asNumber;
  fnVMNumber second = this->value(value[2])->asNumber;

  DEBUG("ADD(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  fnVMNumber sum = first + second;

  return this->declareNumber(sum);
}

fnVMValue* fnVM::fnSubtract(fnByte value[]) {
  // SUBTRACT is of the form:
  // [1-BYTE SUBTRACT_OP_CODE] [1-BYTE CONST REF] [1-BYTE CONST REF]
  //
  // SUBTRACT will return the difference as a Number.
  fnVMNumber first = this->value(value[1])->asNumber;
  fnVMNumber second = this->value(value[2])->asNumber;

  DEBUG("SUBTRACT(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  fnVMNumber difference = first - second;

  return this->declareNumber(difference);
}
