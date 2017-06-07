#pragma once

#include <vector> // std::vector

namespace fn { namespace vm {

  // Def encapsulates a custom function definition in the VM.
  typedef struct _Def {
    bytecode::InstructionIndex length;
    std::vector<bytecode::NameHash> args;
    bytecode::InstructionIndex counterStart;

    bool operator==(_Def other) {
      return this->length == other.length
        && this->args == other.args
        && this->counterStart == other.counterStart;
    }
  } Def;

}}
