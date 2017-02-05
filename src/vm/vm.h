// The Fn VM is a basic, hand-made virtual machine.
// Using a handful of VM instructions,
// this VM is capable of running any Fn program.

#pragma once

#include "stdlib.h" // size_t
#include <vector> // std::vector

#include "src/bytecode.h" // CodeByte
#include "src/vm/value.h" // vm::Value
#include "src/vm/number.h" // vm::Number

namespace fn {

  // VM is the virtual machine that instructions are run in.
  class VM {
  public:

    VM(bool debug);
    VM() : VM(false) {};
    ~VM();

    // Executes a number of instructions.
    // Returns the pointer to the return value.
    vm::Value run(bytecode::CodeByte instructions[], size_t num_bytes);

  protected:
    bool debug;

    std::vector<vm::Value*> values;
    vm::Value* value(bytecode::CodeByte index);

    vm::Value* declareBool(bytecode::CodeByte value);
    vm::Value* declareBool(bool value);
    
    vm::Value* fnAnd(bytecode::CodeByte value[]);
    vm::Value* fnOr(bytecode::CodeByte value[]);
    vm::Value* fnNot(bytecode::CodeByte value[]);

    vm::Value* declareNumber(bytecode::CodeByte value[]);
    vm::Value* declareNumber(vm::Number value);

    vm::Value* fnMultiply(bytecode::CodeByte value[]);
    vm::Value* fnDivide(bytecode::CodeByte value[]);
    vm::Value* fnAdd(bytecode::CodeByte value[]);
    vm::Value* fnSubtract(bytecode::CodeByte value[]);
  };

}




