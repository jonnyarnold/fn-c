#include "src/vm/vm.h"

#include <iostream> // std::cout
#include <bitset> // std::bitset
#include <unordered_set> // std::unordered_set

using namespace fn;

#define DEBUG(msg) if (this->debug) { std::cout << "I" << std::to_string(this->counter) << "> " << msg << std::endl; }

VM::VM(bool debug) {
  this->debug = debug;

  // Because 0 is used by CodeGen for errors,
  // we push a NULL onto the ValueMap to sync.
  this->values = vm::ValueMap();
  this->values.push_back(NULL);

  this->callStack = std::stack<vm::CallFrame>();
}

VM::~VM() {
  // Save the last value,
  // so it can be used externally.
  std::unordered_set<vm::Value*> deletedValues = std::unordered_set<vm::Value*>{
    NULL,
    this->values.back()
  };

  for(auto value : this->values) {
    if (deletedValues.find(value) == deletedValues.end()) {
      deletedValues.insert(value);
      delete value;
    }
  }
}

vm::Value* VM::run(bytecode::CodeBlob* instructions) {
  return this->run(instructions->asBytes(), instructions->size());
}

vm::Value* VM::run(bytecode::CodeByte instructions[], size_t num_bytes) {
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

    case FN_OP_EQ:
      this->fnEq(&instructions[counter]);
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

    // this->printState();
  }

  return this->values.back();
}

// Get a value by index.
vm::Value* VM::value(bytecode::CodeByte index) {
  return this->values[index];
}

bytecode::ValueIndex VM::declare(vm::Value* value) {
  bytecode::ValueIndex index = this->values.size();
  DEBUG("DECLARE(" << value->toString() << ") [V" << std::to_string(index) << "]");
  this->values.push_back(value);
  return index;
}

void VM::printState() {
  if (!this->debug) return;

  std::cout << "VM State:\n";
  std::cout << "counter: I" << std::to_string(this->counter) <<"\n";
  std::cout << "values:\n";
  for (uint i = 1; i < this->values.size(); i++) {
    std::cout << "V" << i << ": " << this->values[i]->toString() << "\n";
  }
  std::cout << "call stack size: " << this->callStack.size() << "\n";
  if (this->callStack.size() > 0) {
    std::cout << "top frame: at exit, returnInto@V" << std::to_string(this->callStack.top().returnIndex)
      << ", returnTo@I" << std::to_string(this->callStack.top().returnCounter) << "\n";
  }
}

// A "fold" is a kind of garbage collection
// that deletes every value up to returnIndex
// and then fills returnIndex with returnValue.
void VM::fold(bytecode::ValueIndex returnIndex, vm::Value* returnValue) {
  while (this->values.size() > returnIndex) { this->values.pop_back(); }
  this->declare(returnValue);
}

// BOOL_TRUE
// BOOL_FALSE
// (1 byte)
//
// Allocates a new boolean constant.
void VM::declareBool(bytecode::CodeByte value) {
  this->declareBool((bool)value);
}

void VM::declareBool(bool value) {
  this->declare(new vm::BoolValue(value));
}

// NUMBER [EXPONENT (1)] [COEFFICIENT (8)]
// (10 bytes)
//
// Allocates a new numeric constant.
void VM::declareNumber(bytecode::CodeByte value[]) {
  Exponent exponent = (Exponent)value[1];
  Coefficient coefficient = (Coefficient)value[2];

  this->declareNumber(Number(exponent, coefficient));
}

void VM::declareNumber(Number value) {
  this->declare(new vm::NumberValue(value));
}

// AND [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns true if both arguments are true,
// false otherwise.
void VM::fnAnd(bytecode::CodeByte value[]) {

  bool first = this->value(value[1])->asBool();
  if (!first) {
    DEBUG("AND(V" << std::to_string(value[1]) << " = false, <unexecuted>)");
    this->declareBool(FN_OP_FALSE);
    return;
  }

  bool second = this->value(value[2])->asBool();
  if (!second) {
    DEBUG("AND(V" << std::to_string(value[1]) << " = true, V" << std::to_string(value[2]) << " = true)");
    this->declareBool(FN_OP_FALSE);
    return;
  }

  DEBUG("OR(V" << std::to_string(value[1]) << " = true, V" << std::to_string(value[2]) << " = true)");
  this->declareBool(FN_OP_TRUE);
}

// OR [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns true if either arguments are true,
// false otherwise.
void VM::fnOr(bytecode::CodeByte value[]) {
  bool first = this->value(value[1])->asBool();
  if (first) {
    DEBUG("OR(V" << std::to_string(value[1]) << " = true, <unexecuted>)");
    this->declareBool(FN_OP_TRUE);
    return;
  }

  bool second = this->value(value[2])->asBool();
  if (second) {
    DEBUG("OR(V" << std::to_string(value[1]) << " = false, V" << std::to_string(value[2]) << " = true)");
    this->declareBool(FN_OP_TRUE);
    return;
  }

  DEBUG("OR(V" << std::to_string(value[1]) << " = false, V" << std::to_string(value[2]) << " = false)");
  this->declareBool(FN_OP_FALSE);
}

// NOT [INDEX (1)]
// (2 bytes)
//
// Returns true if the argument is false
// and vice versa.
void VM::fnNot(bytecode::CodeByte value[]) {
  bool arg = this->value(value[1])->asBool();
  DEBUG("NOT(V" << std::to_string(value[1]) << " = " << arg << ")");

  arg ?
    this->declareBool(FN_OP_FALSE) :
    this->declareBool(FN_OP_TRUE);
}

// MULTIPLY [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the product of two numeric values.
void VM::fnMultiply(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("MULTIPLY(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number product = first * second;

  this->declareNumber(product);
}

// DIVIDE [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the fraction of two numeric values.
void VM::fnDivide(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("DIVIDE(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number fraction = first / second;

  this->declareNumber(fraction);
}

// ADD [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the sum of two numeric values.
void VM::fnAdd(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("ADD(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number sum = first + second;

  this->declareNumber(sum);
}

// SUBTRACT [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns the difference of two numeric values.
void VM::fnSubtract(bytecode::CodeByte value[]) {
  Number first = this->value(value[1])->asNumber();
  Number second = this->value(value[2])->asNumber();

  DEBUG("SUBTRACT(" << (int)first.coefficient << " * 10^" << (int)first.exponent << ", " << (int)second.coefficient << " * 10^" << (int)second.exponent << ")");

  Number difference = first - second;

  this->declareNumber(difference);
}

// EQ [INDEX_1 (1)] [INDEX_2 (1)]
// (3 bytes)
//
// Returns true if the two values are equal,
// false otherwise.
void VM::fnEq(bytecode::CodeByte value[]) {
  vm::Value* first = this->value(value[1]);
  vm::Value* second = this->value(value[2]);

  bool eq = first->eq(second);
  this->declareBool(eq);
}

// LOAD [INDEX (1)]
// (2 bytes)
//
// Returns the value at the given index.
void VM::load(bytecode::CodeByte value[]) {
  bytecode::ValueIndex index = value[1];
  DEBUG("LOAD(V" << (int)index << ")");
  this->declare(this->value(index));
}

#define DECLARE_DEF_BYTES (1 + INSTRUCTION_INDEX_BYTES)
// DECLARE_DEF [LENGTH (8)]
// (9 + LENGTH bytes)
//
// Starts a Def declaration. Reads the body of the Def
// and stores it in a pointer value.
void VM::declareDef(bytecode::CodeByte value[]) {
  bytecode::InstructionIndex length = value[1];
  bytecode::InstructionIndex counterStart = this->counter + DECLARE_DEF_BYTES;

  vm::Def def = vm::Def();
  def.length = length;
  def.counterStart = counterStart;
  this->declareDef(def);

  // Skip over the Def body.
  this->counter += (length + DECLARE_DEF_BYTES);
}

void VM::declareDef(vm::Def value) {
  this->declare(new vm::DefValue(value));
}

#define CALL_BYTES (1 + INSTRUCTION_INDEX_BYTES)
// CALL [INDEX (1)]
// (2 bytes)
//
// Runs the code pointed to at the given index.
void VM::call(bytecode::CodeByte value[]) {
  bytecode::ValueIndex index = value[1];
  vm::Value* def = this->value(index);

  DEBUG("CALL(" << (int)index << ")");

  // Push a CallFrame onto the stack.
  vm::CallFrame frame = vm::CallFrame();
  frame.returnCounter = this->counter + CALL_BYTES;
  frame.returnIndex = this->values.size();
  this->callStack.push(frame);

  // Set the program counter.
  bytecode::InstructionIndex newCounterPos = def->getCallCounterPos();
  this->counter = newCounterPos;
}

// RETURN_LAST
// (1 byte)
//
// Denotes the end of a call. Returns the last defined value.
void VM::returnLast() {
  DEBUG("RETURN_LAST()");

  vm::Value* returnValue = this->values.back();

  // Pop the top CallFrame and free memory used by it.
  vm::CallFrame frame = this->callStack.top();
  this->callStack.pop();

  this->fold(frame.returnIndex, returnValue);
  this->counter = frame.returnCounter;
}
