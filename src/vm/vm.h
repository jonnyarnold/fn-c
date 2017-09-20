// The Fn VM is a basic, hand-made virtual machine.
// Using a handful of VM instructions,
// this VM is capable of running any Fn program.

#pragma once

#include "stdlib.h" // size_t
#include <vector> // std::vector
#include <stack> // std::stack
#include <unordered_map> // std::unordered_map

#include "src/bytecode.h" // CodeByte
#include "src/vm/value.h" // vm::Value
#include "src/vm/def.h" // vm::Def
#include "src/number.h" // Number

namespace fn {

  namespace vm {
    typedef std::vector<vm::Value*> ValueMap;
    typedef std::unordered_map<bytecode::NameHash, vm::Value*> SymbolTable;

    class CallFrame {
    public:
      // Denotes the place the program counter will be set to
      // when RETURN_LAST is hit.
      bytecode::InstructionIndex returnCounter;

      // The values defined in the given frame.
      vm::ValueMap values;

      // The symbols defined in the given frame.
      vm::SymbolTable symbols;

      CallFrame(bytecode::InstructionIndex returnCounter) {
        this->returnCounter = returnCounter;

        // The [] operator returns 0 if a value
        // is not found. To make sure that's not
        // confused with the first element, we push
        // a NULL value to the 0 position.
        this->values = vm::ValueMap{ NULL };
      }

      CallFrame() : CallFrame(-1) {}

      ~CallFrame() {
        for(auto value : this->values) {
          if (value != NULL) { delete value; }
        }
      }
    };
  }

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
  };

}
