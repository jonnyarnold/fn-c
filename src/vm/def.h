#pragma once

namespace fn { namespace vm {
  
  // Def encapsulates a custom function definition in the VM.
  typedef struct _Def {
    bytecode::InstructionIndex length;
    bytecode::InstructionIndex counterStart;
  } Def;

}}
