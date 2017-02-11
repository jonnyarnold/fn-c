#include "src/vm/vm.h"

#include <iostream> // std::cout
#include <bitset> // std::bitset

using namespace fn;

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

VM::VM(bool debug) {
  this->debug = debug;
  this->values = std::vector<vm::Value*>();
}

VM::~VM() {
  this->values.clear();
}

vm::Value VM::run(bytecode::CodeBlob* instructions) {
  return this->run(instructions->asBytes(), instructions->size());
}

vm::Value VM::run(bytecode::CodeByte instructions[], size_t num_bytes) {
  size_t counter = 0;
  vm::Value* returnValue = NULL;

  while(counter < num_bytes) {
    bytecode::CodeByte opcode = instructions[counter];

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

// Get a value by index.
vm::Value* VM::value(bytecode::CodeByte index) {
  return this->values[index];
}

// BOOL_TRUE
// BOOL_FALSE
// (1 byte)
//
// Allocates a new boolean constant.
vm::Value* VM::declareBool(bytecode::CodeByte value) {
  return this->declareBool((bool)value);
}

vm::Value* VM::declareBool(bool value) {
  DEBUG("DECLARE_BOOL(" << value << ")");
  vm::Value* b = new vm::Value;
  b->asBool = value;

  this->values.push_back(b);
  return b;
}

// NUMBER [EXPONENT (1)] [COEFFICIENT (6)]
// (8 bytes)
//
// Allocates a new numeric constant.
vm::Value* VM::declareNumber(bytecode::CodeByte value[]) {
  Exponent exponent = (Exponent)value[1];
  Coefficient coefficient = (Coefficient)value[2];

  DEBUG("DECLARE_NUMBER(" << (int)coefficient << " * 10^" << (int)exponent << ")");

  vm::Value* n = new vm::Value;
  n->asNumber = Number(exponent, coefficient);

  this->values.push_back(n);

  return n;
}

vm::Value* VM::declareNumber(Number value) {
  DEBUG("DECLARE_NUMBER(" << (int)value.coefficient << " * 10^" << (int)value.exponent << ")");

  vm::Value* n = new vm::Value;
  n->asNumber = value;

  this->values.push_back(n);
  return n;
}

// AND [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns true if both arguments are true,
// false otherwise.
vm::Value* VM::fnAnd(bytecode::CodeByte value[]) {
  
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

// OR [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns true if either arguments are true,
// false otherwise.
vm::Value* VM::fnOr(bytecode::CodeByte value[]) {
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

// NOT [INDEX (1)]
// (2 bytes)
//
// Returns true if the argument is false
// and vice versa.
vm::Value* VM::fnNot(bytecode::CodeByte value[]) {
  bool arg = this->value(value[1])->asBool;
  DEBUG("NOT(" << arg << ")");

  if (arg) {
    return this->declareBool(FN_OP_FALSE);
  } else {
    return this->declareBool(FN_OP_TRUE);
  }
}

// MULTIPLY [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the product of two numeric values.
vm::Value* VM::fnMultiply(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber;
  Number second = this->value(value[2])->asNumber;

  DEBUG("MULTIPLY(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number product = first * second;

  return this->declareNumber(product);
}

// DIVIDE [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the fraction of two numeric values.
vm::Value* VM::fnDivide(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber;
  Number second = this->value(value[2])->asNumber;

  DEBUG("DIVIDE(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number fraction = first / second;

  return this->declareNumber(fraction);
}

// ADD [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the sum of two numeric values.
vm::Value* VM::fnAdd(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber;
  Number second = this->value(value[2])->asNumber;

  DEBUG("ADD(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number sum = first + second;

  return this->declareNumber(sum);
}

// SUBTRACT [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the difference of two numeric values.
vm::Value* VM::fnSubtract(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber;
  Number second = this->value(value[2])->asNumber;

  DEBUG("SUBTRACT(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number difference = first - second;

  return this->declareNumber(difference);
}
