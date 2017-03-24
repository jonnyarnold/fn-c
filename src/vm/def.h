#pragma once

namespace fn { namespace vm {

  // Def encapsulates a custom function definition in the VM.
  typedef struct _Def {
    bytecode::InstructionIndex length;
    bytecode::InstructionIndex counterStart;

    bool operator==(_Def other) {
      return this->length == other.length
        && this->counterStart == other.counterStart;
    }
  } Def;

}}
