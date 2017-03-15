// The Fn VM is a basic, hand-made virtual machine.
// Using a handful of VM instructions,
// this VM is capable of running any Fn program.

#pragma once

#include "stdlib.h" // size_t
#include <vector> // std::vector
#include <stack> // std::stack

#include "src/bytecode.h" // CodeByte
#include "src/vm/value.h" // vm::Value
#include "src/vm/def.h" // vm::Def
#include "src/number.h" // Number

namespace fn {

  namespace vm {
    typedef std::vector<vm::Value*> ValueMap;

    // CallFrame defines a call frame (an invocation of a function).
    typedef struct _CallFrame {

      // Denotes the place the program counter will be set to
      // when RETURN_LAST is hit.
      bytecode::InstructionIndex returnCounter;

      // Denotes the index that the result of the call will be placed into.
      bytecode::ValueIndex returnIndex;

    } CallFrame;
  }

  // VM is the virtual machine that instructions are run in.
  class VM {
  public:

    VM(bool debug);
    VM() : VM(false) {};
    // ~VM();

    // Executes a number of instructions.
    // Returns the pointer to the return value.
    vm::Value run(bytecode::CodeBlob* instructions);
    vm::Value run(bytecode::CodeByte instructions[], size_t num_bytes);

  protected:
    bool debug;

    // Program counter.
    bytecode::InstructionIndex counter;

    // Allows conversion from identifier to index.
    vm::ValueMap values;

    // Stores the nesting of function calls.
    std::stack<vm::CallFrame> callStack;
    
    vm::Value* value(bytecode::CodeByte index);
    vm::Value* declare(vm::Value* value);
    void printState();

    vm::Value* declareBool(bytecode::CodeByte);
    vm::Value* declareBool(bool value);
    
    vm::Value* fnAnd(bytecode::CodeByte[]);
    vm::Value* fnOr(bytecode::CodeByte[]);
    vm::Value* fnNot(bytecode::CodeByte[]);

    vm::Value* declareNumber(bytecode::CodeByte[]);
    vm::Value* declareNumber(Number);

    vm::Value* fnMultiply(bytecode::CodeByte[]);
    vm::Value* fnDivide(bytecode::CodeByte[]);
    vm::Value* fnAdd(bytecode::CodeByte[]);
    vm::Value* fnSubtract(bytecode::CodeByte[]);

    vm::Value* load(bytecode::CodeByte[]);

    vm::Value* declareDef(bytecode::CodeByte[]);
    vm::Value* declareDef(vm::Def def);
    vm::Value* call(bytecode::CodeByte[]);
    vm::Value* returnLast();
  };

}
