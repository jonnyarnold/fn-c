// The Fn VM is a basic, hand-made virtual machine.
// Using a handful of VM instructions,
// this VM is capable of running any Fn program.

#pragma once

#include <vector> // std::vector
#include <unordered_map> // std::unordered_map

#define INITIAL_VALUE_MAP_CAPACITY 4
#define INITIAL_SYMBOL_TABLE_CAPACITY 16

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

      CallFrame(bytecode::InstructionIndex returnCounter) : CallFrame() {
        this->returnCounter = returnCounter;
      }

      CallFrame() {
        // The [] operator returns 0 if a value
        // is not found. To make sure that's not
        // confused with the first element, we push
        // a NULL value to the 0 position.
        this->values = vm::ValueMap();
        this->values.reserve(INITIAL_VALUE_MAP_CAPACITY);
        this->values.push_back(NULL);

        this->symbols = vm::SymbolTable();
        this->symbols.reserve(INITIAL_SYMBOL_TABLE_CAPACITY);
      }

      ~CallFrame() {
        for(auto value : this->values) {
          if (value != NULL) { delete value; }
        }
      }
    };
  }
}
