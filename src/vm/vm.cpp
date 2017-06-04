#include "src/vm/vm.h"

#include <iostream> // std::cout
#include <bitset> // std::bitset
#include <unordered_set> // std::unordered_set

using namespace fn;

#define DEBUG(msg) if (this->debug) { std::cout << "I" << std::to_string(this->counter) << "> " << msg << std::endl; }

VM::VM(bool debug) {
  this->debug = debug;
  this->callStack = std::stack<vm::CallFrame*>();
}

vm::Value* VM::run(bytecode::CodeBlob* instructions) {
  return this->run(instructions->asBytes(), instructions->size());
}

vm::Value* VM::run(bytecode::CodeByte instructions[], size_t num_bytes) {
  this->counter = 0;
  this->callStack.push(new vm::CallFrame());

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

    case FN_OP_WHEN_HEADER:
      this->beginWhen(&instructions[counter]);
      // counter is moved by beginWhen
      break;

    case FN_OP_FALSE_JUMP:
      this->jumpIfLastFalse(&instructions[counter]);
      // counter is moved by jumpIfLastFalse
      break;

    default:
      throw "Unexpected opcode"; // TODO: Make this more meaningful

    }

    // this->printState();
  }

  return this->lastValue();
}

// Get a value by index.
vm::Value* VM::value(bytecode::CodeByte index) {
  return this->currentFrame()->values[index];
}

bytecode::ValueIndex VM::declare(vm::Value* value) {
  bytecode::ValueIndex index = this->currentFrame()->values.size();
  DEBUG("DECLARE(" << value->toString() << ") [V" << std::to_string(index) << "]");
  this->currentFrame()->values.push_back(value);
  return index;
}

void VM::printState() {
  if (!this->debug) return;

  std::cout << "VM State:\n";
  std::cout << "counter: I" << std::to_string(this->counter) <<"\n";
  std::cout << "values:\n";
  for (uint i = 1; i < this->currentFrame()->values.size(); i++) {
    std::cout << "V" << i << ": " << this->currentFrame()->values[i]->toString() << "\n";
  }
  std::cout << "call stack size: " << this->callStack.size() << "\n";
  if (this->callStack.size() > 0) {
    std::cout << "top frame: at exit, returnTo@I" << std::to_string(this->currentFrame()->returnCounter) << "\n";
  }
}

vm::Value* VM::lastValue() {
  return this->currentFrame()->values.back();
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

  DEBUG("EQ(V" << std::to_string(value[1]) << ", V" << std::to_string(value[2]) << ")");

  bool eq = first->eq(second);

  // DEBUG("EQ(" << first->toString() << ", " << second->toString() << ") = " << std::to_string(eq));

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

// CALL [INDEX (1)] [NUM_ARGS (1)] [ARG_INDEX (1)*]
// (3 + NUM_ARGS bytes)
//
// Runs the code pointed to at the given index.
void VM::call(bytecode::CodeByte value[]) {
  bytecode::ValueIndex index = value[1];
  bytecode::ValueIndex numArgs = value[2];
  bytecode::ValueIndex* argIndices = &value[3];
  bytecode::ValueIndex headerBytes = 3 + numArgs;

  DEBUG("CALL (V" << std::to_string(index) << " with " << std::to_string(numArgs) << " args)");

  vm::Value* def = this->value(index);

  // Push a CallFrame onto the stack.
  vm::CallFrame* frame = new vm::CallFrame();
  frame->returnCounter = this->counter + headerBytes;

  // V1 is the current definition
  frame->values.push_back(def);

  // Vx are the arguments
  for(uint i = 0; i < numArgs; i++) {
    frame->values.push_back(this->value(argIndices[i]));
  }

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

  // We transfer ownership of the value from the inner call to the outer call.
  vm::Value* returnValue = this->currentFrame()->values.back();
  this->currentFrame()->values.pop_back();

  this->counter = this->currentFrame()->returnCounter;

  // Pop the top CallFrame and free memory used by it.
  vm::CallFrame* frame = this->currentFrame();
  this->callStack.pop();

  // Declare the return value in the new current frame.
  this->declare(returnValue);

  // TODO: How do we make sure that args don't get deleted,
  // or that deleting them doesn't destroy them on the parent?
  // delete frame;
}

// WHEN_HEADER [LENGTH (8)]
// (9 bytes)
//
// Denotes the start of a conditional.
#define WHEN_HEADER_BYTES (1 + INSTRUCTION_INDEX_BYTES)
void VM::beginWhen(bytecode::CodeByte value[]) {
  bytecode::InstructionIndex length = value[1];

  DEBUG("WHEN_HEADER(" << std::to_string(length) << ")");

  // Push a CallFrame onto the stack.
  // TODO: Do something better than copying the existing call frame.
  vm::CallFrame* frame = new vm::CallFrame(*this->currentFrame());
  frame->returnCounter = this->counter + WHEN_HEADER_BYTES + length;
  this->callStack.push(frame);

  this->counter += WHEN_HEADER_BYTES;
}

// FALSE_JUMP [OFFSET (8)]
// (9 bytes)
//
// If the last value was false, jump the number of bytes given.
#define FALSE_JUMP_BYTES (1 + INSTRUCTION_INDEX_BYTES)
void VM::jumpIfLastFalse(bytecode::CodeByte value[]) {
  bytecode::InstructionIndex jump = value[1];

  if(this->lastValue()->asBool() == false) {
    DEBUG("FALSE_JUMP(" << std::to_string(jump) << ") = JUMP");
    this->counter += FALSE_JUMP_BYTES + jump;
    return;
  }

  // else...
  DEBUG("FALSE_JUMP(" << std::to_string(jump) << ") = NO JUMP");
  this->counter += FALSE_JUMP_BYTES;
}
