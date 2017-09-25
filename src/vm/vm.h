// The Fn VM is a basic, hand-made virtual machine.
// Using a handful of VM instructions,
// this VM is capable of running any Fn program.

#pragma once

#include "stdlib.h" // size_t

#include "src/bytecode.h" // CodeByte
#include "src/vm/value.h" // vm::Value
#include "src/vm/def.h" // vm::Def
#include "src/number.h" // Number

#include "src/vm/callframe.h" // vm::CallFrame

#define INITIAL_VALUE_MAP_CAPACITY 4
#define INITIAL_SYMBOL_TABLE_CAPACITY 16

namespace fn {

  // VM is the virtual machine that instructions are run in.
  class VM {
  public:

    VM(bool debug);
    VM() : VM(false) {};
    // ~VM();

    // Executes a number of instructions.
    // Returns the pointer to the return value.
    vm::Value* run(bytecode::CodeBlob* instructions);
    vm::Value* run(bytecode::CodeByte instructions[], size_t num_bytes);

  protected:
    bool debug;

    // Program counter.
    bytecode::InstructionIndex counter;

    // Stores the nesting of function calls.
    std::vector<vm::CallFrame*> callStack;
    void pushFrame(vm::CallFrame*);
    void popFrame();
    vm::CallFrame* currentFrame;

    void pushValue(vm::Value* value);
    vm::Value* popValue();
    vm::Value* lastValue();

    void printState();

    void declareBool(bytecode::CodeByte);
    void declareBool(bool value);

    void fnAnd();
    void fnOr();
    void fnNot();

    void declareNumber(bytecode::CodeByte[]);
    void declareNumber(Number);

    void fnMultiply();
    void fnDivide();
    void fnAdd();
    void fnSubtract();

    void fnEq();

    void name(bytecode::CodeByte value[]);
    void load(bytecode::CodeByte[]);

    void declareDef(bytecode::CodeByte[]);
    void declareDef(vm::Def def);
    void call();
    void returnLast();

    void jumpIfLastFalse(bytecode::CodeByte[]);

    // Frame manipulation.
    void newFrame();
    void compress();
    void expand();
  };

}
