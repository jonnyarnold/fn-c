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

    class CallFrame {
    public:
      // Denotes the place the program counter will be set to
      // when RETURN_LAST is hit.
      bytecode::InstructionIndex returnCounter;

      // The values defined in the given frame.
      vm::ValueMap values;

      CallFrame(bytecode::InstructionIndex returnCounter) {
        this->returnCounter = returnCounter;

        // The [] operator returns 0 if a value
        // is not found. To make sure that's not
        // confused with the first element, we push
        // a NULL value to the 0 position.
        this->values = vm::ValueMap();
        this->values.push_back(NULL);
      }

      CallFrame() : CallFrame(-1) {}

      ~CallFrame() {
        for(auto value : this->values) {
          if (value != NULL && value != this->values.back()) { delete value; }
        }
      }
    };
  }

  // VM is the virtual machine that instructions are run in.
  class VM {
  public:

    VM(bool debug);
    VM() : VM(false) {};
    ~VM();

    // Executes a number of instructions.
    // Returns the pointer to the return value.
    vm::Value* run(bytecode::CodeBlob* instructions);
    vm::Value* run(bytecode::CodeByte instructions[], size_t num_bytes);

  protected:
    bool debug;

    // Program counter.
    bytecode::InstructionIndex counter;

    // Stores the nesting of function calls.
    std::stack<vm::CallFrame*> callStack;
    vm::CallFrame* currentFrame() { return this->callStack.top(); }

    vm::Value* value(bytecode::CodeByte index);
    bytecode::ValueIndex declare(vm::Value* value);
    void printState();
    void fold(bytecode::ValueIndex returnIndex, vm::Value* returnValue);
    vm::Value* lastValue();

    void declareBool(bytecode::CodeByte);
    void declareBool(bool value);

    void fnAnd(bytecode::CodeByte[]);
    void fnOr(bytecode::CodeByte[]);
    void fnNot(bytecode::CodeByte[]);

    void declareNumber(bytecode::CodeByte[]);
    void declareNumber(Number);

    void fnMultiply(bytecode::CodeByte[]);
    void fnDivide(bytecode::CodeByte[]);
    void fnAdd(bytecode::CodeByte[]);
    void fnSubtract(bytecode::CodeByte[]);

    void fnEq(bytecode::CodeByte[]);

    void load(bytecode::CodeByte[]);

    void declareDef(bytecode::CodeByte[]);
    void declareDef(vm::Def def);
    void call(bytecode::CodeByte[]);
    void returnLast();
  };

}
