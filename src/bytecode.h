// Fn Bytecode is the intermediate representation of Fn programs.
// It is generated by the code generator and executed by the
// virtual machine.

#pragma once

namespace fn { namespace bytecode {

  // The smallest unit of instruction.
  // (Note that instructions are at least this size,
  // but are usually more.)
  typedef char CodeByte;

  // The first byte of any instruction is an OpCode.
  // It tells the VM the operation to perform.
  typedef CodeByte OpCode;
  #define FN_OP_FALSE (fn::bytecode::OpCode)(0)
  #define FN_OP_TRUE (fn::bytecode::OpCode)(1)
  #define FN_OP_AND (fn::bytecode::OpCode)(2)
  #define FN_OP_OR (fn::bytecode::OpCode)(3)
  #define FN_OP_NOT (fn::bytecode::OpCode)(4)

  #define FN_OP_NUMBER (fn::bytecode::OpCode)(5)
  #define FN_OP_MULTIPLY (fn::bytecode::OpCode)(6)
  #define FN_OP_DIVIDE (fn::bytecode::OpCode)(7)
  #define FN_OP_ADD (fn::bytecode::OpCode)(8)
  #define FN_OP_SUBTRACT (fn::bytecode::OpCode)(9)

  #define FN_OP_STRING (fn::bytecode::OpCode)(10)

  // TODO: Instruction generators

}}