#include "src/vm/vm.h"

#include <iostream> // std::cout
#include <bitset> // std::bitset
#include <unordered_set> // std::unordered_set

using namespace fn;

#define DEBUG(msg) if (this->debug) { std::cout << "I" << std::to_string(this->counter) << "> " << msg << std::endl; }

VM::VM(bool debug) {
  this->debug = debug;
  this->callStack = std::vector<vm::CallFrame*>();
}

vm::Value* VM::run(bytecode::CodeBlob* instructions) {
  return this->run(instructions->asBytes(), instructions->size());
}

vm::Value* VM::run(bytecode::CodeByte instructions[], size_t num_bytes) {
  this->counter = 0;
  this->pushFrame(new vm::CallFrame(num_bytes));

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
      this->fnAnd();
      this->counter += 1;
      break;

    case FN_OP_OR:
      this->fnOr();
      this->counter += 1;
      break;

    case FN_OP_NOT:
      this->fnNot();
      this->counter += 1;
      break;

    case FN_OP_MULTIPLY:
      this->fnMultiply();
      this->counter += 1;
      break;

    case FN_OP_DIVIDE:
      this->fnDivide();
      this->counter += 1;
      break;

    case FN_OP_ADD:
      this->fnAdd();
      this->counter += 1;
      break;

    case FN_OP_SUBTRACT:
      this->fnSubtract();
      this->counter += 1;
      break;

    case FN_OP_EQ:
      this->fnEq();
      this->counter += 1;
      break;

    case FN_OP_NAME:
      this->name(&instructions[counter]);
      this->counter += 2;
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
      this->call();
      // counter is moved by call
      break;

    case FN_OP_RETURN_LAST:
      this->returnLast();
      // counter is moved by returnLast
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

void VM::pushFrame(vm::CallFrame* frame) {
  this->callStack.push_back(frame);
  this->currentFrame = frame;
}

void VM::popFrame() {
  this->callStack.pop_back();
  this->currentFrame = this->callStack.back();
}

void VM::pushValue(vm::Value* value) {
  DEBUG("PUSH(" << value->toString() << ")");
  this->currentFrame->values.push_back(value);
}

vm::Value* VM::popValue() {
  vm::Value* value = this->currentFrame->values.back();
  this->currentFrame->values.pop_back();
  return value;
}

void VM::printState() {
  if (!this->debug) return;

  std::cout << "VM State:\n";
  std::cout << "counter: I" << std::to_string(this->counter) <<"\n";
  std::cout << "values:\n";
  for (uint i = 1; i < this->currentFrame->values.size(); i++) {
    std::cout << "V" << i << ": " << this->currentFrame->values[i]->toString() << "\n";
  }
  std::cout << "call stack size: " << this->callStack.size() << "\n";
  if (this->callStack.size() > 0) {
    std::cout << "top frame: at exit, returnTo@I" << std::to_string(this->currentFrame->returnCounter) << "\n";
  }
}

vm::Value* VM::lastValue() {
  return this->currentFrame->values.back();
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
  this->pushValue(new vm::BoolValue(value));
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
  this->pushValue(new vm::NumberValue(value));
}

// AND
// (1 byte)
//
// Returns true if the top two values on the stack are true,
// false otherwise.
void VM::fnAnd() {
  bool first = this->popValue()->asBool();
  bool second = this->popValue()->asBool();
  DEBUG("AND");

  if (first && second) {
    this->declareBool(FN_OP_TRUE);
  } else {
    this->declareBool(FN_OP_FALSE);
  }
}

// OR
// (1 byte)
//
// Returns true if either of the top two values on the stack are true,
// false otherwise.
void VM::fnOr() {
  bool first = this->popValue()->asBool();
  bool second = this->popValue()->asBool();
  DEBUG("OR");

  if (first || second) {
    this->declareBool(FN_OP_TRUE);
  } else {
    this->declareBool(FN_OP_FALSE);
  }
}

// NOT
// (1 byte)
//
// Returns true if the top value on the stack is false
// and vice versa.
void VM::fnNot() {
  bool arg = this->popValue()->asBool();
  DEBUG("NOT");

  arg ?
    this->declareBool(FN_OP_FALSE) :
    this->declareBool(FN_OP_TRUE);
}

// MULTIPLY
// (1 byte)
//
// Returns the product of two numeric values.
void VM::fnMultiply() {
  Number first = this->popValue()->asNumber();
  Number second = this->popValue()->asNumber();

  DEBUG("MULTIPLY");

  Number product = first * second;

  this->declareNumber(product);
}

// DIVIDE
// (1 byte)
//
// Returns the fraction of two numeric values.
void VM::fnDivide() {
  Number first = this->popValue()->asNumber();
  Number second = this->popValue()->asNumber();

  DEBUG("DIVIDE");

  Number fraction = first / second;

  this->declareNumber(fraction);
}

// ADD
// (1 byte)
//
// Returns the sum of two numeric values.
void VM::fnAdd() {
  Number first = this->popValue()->asNumber();
  Number second = this->popValue()->asNumber();

  DEBUG("ADD");

  Number sum = first + second;

  this->declareNumber(sum);
}

// SUBTRACT
// (1 byte)
//
// Returns the difference of two numeric values.
void VM::fnSubtract() {
  Number first = this->popValue()->asNumber();
  Number second = this->popValue()->asNumber();

  DEBUG("SUBTRACT");

  Number difference = first - second;

  this->declareNumber(difference);
}

// EQ
// (1 byte)
//
// Returns true if the two values are equal,
// false otherwise.
void VM::fnEq() {
  vm::Value* first = this->popValue();
  vm::Value* second = this->popValue();

  DEBUG("EQ");

  bool eq = first->eq(second);

  this->declareBool(eq);
}

// NAME [NAME (1)]
// (2 bytes)
//
// Sets a reference to the top of the value stack by the given name.
#define NAME_BYTES (1 + NAME_HASH_BYTES)
void VM::name(bytecode::CodeByte value[]) {
  bytecode::NameHash name = value[1];

  DEBUG("NAME(" << (int)name << ")");

  this->currentFrame->symbols[name] = this->lastValue();
}


// LOAD [NAME (1)]
// (2 bytes)
//
// Returns the value defined by the given name.
// Names are checked in frame order.
void VM::load(bytecode::CodeByte value[]) {
  bytecode::NameHash name = value[1];

  DEBUG("LOAD(" << (int)name << ")");

  // Check each frame, top to bottom.
  vm::Value* foundValue = NULL;
  for(auto frame = this->callStack.rbegin(); frame != this->callStack.rend(); ++frame) {
    if ((*frame)->symbols[name] != NULL) {
      foundValue = (*frame)->symbols[name];
      break;
    }
  }

  if (foundValue != NULL) {
    this->pushValue(foundValue);
  } else {
    throw "Cannot find variable";
  }
}

// DECLARE_DEF [LENGTH (1)] [NUM_ARGS (1)] [ARG_NAMES (1)*]
// (10 + LENGTH + NUM_ARGS bytes)
//
// Starts a Def declaration. Reads the body of the Def
// and stores it in a pointer value.
void VM::declareDef(bytecode::CodeByte value[]) {
  bytecode::InstructionIndex length = value[1];
  bytecode::NumArgs numArgs = value[1 + INSTRUCTION_INDEX_BYTES];
  DEBUG("DEF(" << (int)length << " bytes, " << (int)numArgs << " args)");
  
  std::vector<bytecode::NameHash> args = std::vector<bytecode::NameHash>();
  for(uint argIndex = 0; argIndex < numArgs; argIndex++) {
    args.push_back(value[1 + INSTRUCTION_INDEX_BYTES + NUM_ARGS_BYTES + (argIndex * NAME_HASH_BYTES)]);
  }

  bytecode::InstructionIndex headerSize = 1 + INSTRUCTION_INDEX_BYTES + NUM_ARGS_BYTES + (numArgs * NAME_HASH_BYTES);
  bytecode::InstructionIndex counterStart = this->counter + headerSize;

  vm::Def def = vm::Def();
  def.length = length;
  def.args = args;
  def.counterStart = counterStart;
  this->declareDef(def);

  // Skip over the Def body.
  this->counter += (headerSize + length);
}

void VM::declareDef(vm::Def value) {
  this->pushValue(new vm::DefValue(value));
}

// CALL
// (1 byte)
//
// Runs the top of the value stack as a function.
void VM::call() {
  DEBUG("CALL");
  vm::Def def = this->popValue()->asDef();

  // Push a CallFrame onto the stack.
  vm::CallFrame* frame = new vm::CallFrame();
  frame->returnCounter = this->counter + 1;
  
  // Set up ValueStack and SymbolTable
  std::vector<bytecode::NameHash> argNames = def.args;
  for(auto argName : argNames) {
    vm::Value* value = this->popValue();
    frame->symbols[argName] = value;
  }

  this->pushFrame(frame);

  // Set the program counter.
  bytecode::InstructionIndex newCounterPos = def.counterStart;
  this->counter = newCounterPos;
}

// RETURN_LAST
// (1 byte)
//
// Denotes the end of a call. Returns the last defined value.
void VM::returnLast() {
  DEBUG("RETURN_LAST");

  // We transfer ownership of the value from the inner call to the outer call.
  vm::Value* returnValue = this->currentFrame->values.back();
  this->currentFrame->values.pop_back();

  this->counter = this->currentFrame->returnCounter;

  // Pop the top CallFrame and free memory used by it.
  vm::CallFrame* frame = this->currentFrame;

  if (this->callStack.size() > 1) { this->popFrame(); }

  // Declare the return value in the new current frame.
  this->pushValue(returnValue);

  // TODO: How do we make sure that args don't get deleted,
  // or that deleting them doesn't destroy them on the parent?
  // delete frame;
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
