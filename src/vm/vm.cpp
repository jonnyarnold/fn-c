#include "src/vm/vm.h"

#include <iostream> // std::cout
#include <bitset> // std::bitset

using namespace fn;

#define DEBUG(msg) if (this->debug) { std::cout << "I" << std::to_string(this->counter) << "> " << msg << std::endl; }

VM::VM(bool debug) {
  this->debug = debug;
  this->values = vm::ValueMap();
  this->callStack = std::stack<vm::CallFrame>();
}

VM::~VM() {
  for(auto value : this->values) { delete value; }
}

vm::Value VM::run(bytecode::CodeBlob* instructions) {
  return this->run(instructions->asBytes(), instructions->size());
}

vm::Value VM::run(bytecode::CodeByte instructions[], size_t num_bytes) {
  this->counter = 0;

  while(counter < num_bytes) {
    bytecode::CodeByte opcode = instructions[counter];

    switch(opcode) {

    case FN_OP_FALSE:
    case FN_OP_TRUE:
      this->declareBool(opcode);
      this->counter += 1;
      break;

    case FN_OP_NUMBER:
      this->declareNumber(&instructions[counter]);
      this->counter += 10;
      break;

    case FN_OP_AND:
      this->fnAnd(&instructions[counter]);
      this->counter += 3;
      break;

    case FN_OP_OR:
      this->fnOr(&instructions[counter]);
      this->counter += 3;
      break;

    case FN_OP_NOT:
      this->fnNot(&instructions[counter]);
      this->counter += 2;
      break;

    case FN_OP_MULTIPLY:
      this->fnMultiply(&instructions[counter]);
      this->counter += 3;
      break;

    case FN_OP_DIVIDE:
      this->fnDivide(&instructions[counter]);
      this->counter += 3;
      break;

    case FN_OP_ADD:
      this->fnAdd(&instructions[counter]);
      this->counter += 3;
      break;

    case FN_OP_SUBTRACT:
      this->fnSubtract(&instructions[counter]);
      this->counter += 3;
      break;

    case FN_OP_LOAD:
      this->load(&instructions[counter]);
      this->counter += 2;
      break;

    case FN_OP_DEF:
      this->declareDef(&instructions[counter]);
      // counter is moved by declareDef
      break;

    case FN_OP_CALL:
      this->call(&instructions[counter]);
      // counter is moved by call
      break;

    case FN_OP_RETURN_LAST:
      this->returnLast();
      // counter is moved by returnLast
      break;

    default:
      throw "Unexpected opcode"; // TODO: Make this more meaningful

    }

    // if (this->debug) { this->printState(); }
  }

  return *this->values.back();
}

// Get a value by index.
vm::Value* VM::value(bytecode::CodeByte index) {
  return this->values[index];
}

vm::Value* VM::declare(vm::Value* value) {
  DEBUG("DECLARE(" << value->toString() << ") [V" << this->values.size() << "]");
  this->values.push_back(value);
  return value;
}

void VM::printState() {
  if (!this->debug) return;

  std::cout << "VM State:\n";
  std::cout << "counter: I" << std::to_string(this->counter) <<"\n";
  std::cout << "values:\n";
  for (uint i = 0; i < this->values.size(); i++) {
    std::cout << "V" << i << ": " << this->values[i]->toString() << "\n";
  }
  std::cout << "call stack size: " << this->callStack.size() << "\n";
  if (this->callStack.size() > 0) {
    std::cout << "top frame: at exit, returnInto@V" << std::to_string(this->callStack.top().returnIndex)
      << ", returnTo@I" << std::to_string(this->callStack.top().returnCounter) << "\n";
  }
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
  return this->declare(new vm::Value((bool)value));
}

// NUMBER [EXPONENT (1)] [COEFFICIENT (8)]
// (10 bytes)
//
// Allocates a new numeric constant.
vm::Value* VM::declareNumber(bytecode::CodeByte value[]) {
  Exponent exponent = (Exponent)value[1];
  Coefficient coefficient = (Coefficient)value[2];

  return this->declareNumber(Number(exponent, coefficient));
}

vm::Value* VM::declareNumber(Number value) {
  return this->declare(new vm::Value(value));
}

// AND [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns true if both arguments are true,
// false otherwise.
vm::Value* VM::fnAnd(bytecode::CodeByte value[]) {
  
  bool first = this->value(value[1])->asBool();
  if (!first) {
    DEBUG("AND(false, ???)");
    return this->declareBool(FN_OP_FALSE);
  }

  bool second = this->value(value[2])->asBool();
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
  bool first = this->value(value[1])->asBool();
  if (first) {
    DEBUG("OR(true, ???)");
    return this->declareBool(FN_OP_TRUE);
  }

  bool second = this->value(value[2])->asBool();
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
  bool arg = this->value(value[1])->asBool();
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
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("MULTIPLY(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number product = first * second;

  return this->declareNumber(product);
}

// DIVIDE [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the fraction of two numeric values.
vm::Value* VM::fnDivide(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("DIVIDE(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number fraction = first / second;

  return this->declareNumber(fraction);
}

// ADD [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the sum of two numeric values.
vm::Value* VM::fnAdd(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("ADD(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number sum = first + second;

  return this->declareNumber(sum);
}

// SUBTRACT [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the difference of two numeric values.
vm::Value* VM::fnSubtract(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("SUBTRACT(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number difference = first - second;

  return this->declareNumber(difference);
}

// // SAVE_LAST_VALUE [INDEX (1)]
// // (2 bytes)
// //
// // Sets the value at a given index to the last returned value.
// vm::Value* VM::saveLastValue(bytecode::CodeByte value[]) {
//   bytecode::ValueIndex index = value[1];

//   DEBUG("SAVE_LAST_VALUE(" << (int)index << ")");
//   return this->values[index] = new vm::Value(this->lastValue);
// }

// LOAD [INDEX (1)]
// (2 bytes)
//
// Returns the value at the given index.
vm::Value* VM::load(bytecode::CodeByte value[]) {
  bytecode::ValueIndex index = value[1];
  DEBUG("LOAD(V" << (int)index << ")");
  return this->value(index);
}

#define DECLARE_DEF_BYTES (1 + INSTRUCTION_INDEX_BYTES)
// DECLARE_DEF [LENGTH (8)]
// (9 + LENGTH bytes)
//
// Starts a Def declaration. Reads the body of the Def
// and stores it in a pointer value.
vm::Value* VM::declareDef(bytecode::CodeByte value[]) {
  bytecode::InstructionIndex length = value[1];
  bytecode::InstructionIndex counterStart = this->counter + DECLARE_DEF_BYTES;

  vm::Def def = vm::Def();
  def.length = length;
  def.counterStart = counterStart;
  vm::Value* returnValue = this->declareDef(def);

  // Skip over the Def body.
  this->counter += (length + DECLARE_DEF_BYTES);

  return returnValue;
}

vm::Value* VM::declareDef(vm::Def value) {
  return this->declare(new vm::Value(value));
}

#define CALL_BYTES (1 + INSTRUCTION_INDEX_BYTES)
// CALL [INDEX (1)]
// (2 bytes)
//
// Runs the code pointed to at the given index.
vm::Value* VM::call(bytecode::CodeByte value[]) {
  bytecode::ValueIndex index = value[1];
  vm::Def def = this->value(index)->asDef();

  DEBUG("CALL(" << (int)index << ")");
  
  // Push a CallFrame onto the stack.
  vm::CallFrame frame = vm::CallFrame();
  frame.returnCounter = this->counter + CALL_BYTES;
  frame.returnIndex = this->values.size();
  this->callStack.push(frame);

  // Set the program counter.
  this->counter = def.counterStart;

  return NULL; // FIXME!
}

// RETURN_LAST
// (1 byte)
//
// Denotes the end of a call. Returns the last defined value.
vm::Value* VM::returnLast() {
  DEBUG("RETURN_LAST()");

  vm::Value* returnValue = this->values.back();

  // Pop the top CallFrame and free memory used by it.
  vm::CallFrame frame = this->callStack.top();
  this->callStack.pop();

  // Delete all variables used for calculating the return value,
  // and place the return value in the index given by the call frame.
  while (this->values.size() > frame.returnIndex) { this->values.pop_back(); }
  this->values.push_back(returnValue);

  this->counter = frame.returnCounter;

  return returnValue;
}
