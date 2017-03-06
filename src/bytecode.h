// Fn Bytecode is the intermediate representation of Fn programs.
// It is generated by the code generator and executed by the
// virtual machine.

#pragma once

#include <vector>
#include <array>
#include <initializer_list>

#include "src/number.h"

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

  #define FN_OP_SAVE_LAST_VALUE (fn::bytecode::OpCode)(11)
  #define FN_OP_LOAD (fn::bytecode::OpCode)(12)

  // References to values are given by this type.
  typedef CodeByte ValueIndex;

  // A CodeBlob is a contiguous list of CodeBytes.
  class CodeBlob : public std::vector<CodeByte> {

  public:
    CodeBlob() : std::vector<CodeByte>() {}
    CodeBlob(std::initializer_list<CodeByte> bytes) : std::vector<CodeByte>(bytes) {}
    // ~CodeBlob() { this->clear(); }

    // Special constructor taking a list of CodeBlobs.
    // Stores the concatenation of all given CodeBlobs.
    CodeBlob(std::initializer_list<CodeBlob> blobs) : std::vector<CodeByte>() { 
      for (auto argBlob : blobs) {
        this->insert(this->end(), argBlob.begin(), argBlob.end());
      }
    }

    // Create a CodeBlob from an array.
    template<size_t S>
    CodeBlob(std::array<CodeByte, S> bytes) : std::vector<CodeByte>(bytes.begin(), bytes.end()) {}

    // Copies one blob to the end of another.
    void append(CodeBlob blob) {
      this->insert(this->end(), blob.begin(), blob.end());
    }

    // Use asBytes() and size() to work with the bytes directly
    // as a C-array.
    CodeByte* asBytes() { return this->data(); }
  };

  // Instruction Generators
  CodeBlob iFalse();
  CodeBlob iTrue();
  CodeBlob iAnd(ValueIndex first, ValueIndex second);
  CodeBlob iOr(ValueIndex first, ValueIndex second);
  CodeBlob iNot(ValueIndex idx);

  CodeBlob iNumber(Coefficient coefficient, Exponent exponent);
  CodeBlob iMultiply(ValueIndex first, ValueIndex second);
  CodeBlob iDivide(ValueIndex first, ValueIndex second);
  CodeBlob iAdd(ValueIndex first, ValueIndex second);
  CodeBlob iSubtract(ValueIndex first, ValueIndex second);

  CodeBlob iSaveLastValue(ValueIndex index);
  CodeBlob iLoad(ValueIndex index);
}}
