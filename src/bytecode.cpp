#include "src/bytecode.h"

namespace fn { namespace bytecode {

  CodeBlob iFalse() { return CodeBlob{FN_OP_FALSE}; }

  CodeBlob iTrue() { return CodeBlob{FN_OP_TRUE}; }

  CodeBlob iAnd(ValueIndex first, ValueIndex second) { 
    return CodeBlob{FN_OP_AND, first, second}; 
  }

  CodeBlob iOr(ValueIndex first, ValueIndex second) { 
    return CodeBlob{FN_OP_OR, first, second}; 
  }
  
  CodeBlob iNot(ValueIndex idx) { 
    return CodeBlob{FN_OP_NOT, idx}; 
  }

  CodeBlob iNumber(Coefficient coefficient, Exponent exponent) { 
    std::array<CodeByte, 10> bytes = std::array<CodeByte, 10>();
    bytes[0] = FN_OP_NUMBER;
    bytes[1] = exponent;
    bytes[2] = coefficient;

    return CodeBlob(bytes);
  }

  CodeBlob iMultiply(ValueIndex first, ValueIndex second) { 
    return CodeBlob{FN_OP_MULTIPLY, first, second}; 
  }

  CodeBlob iDivide(ValueIndex first, ValueIndex second) { 
    return CodeBlob{FN_OP_DIVIDE, first, second}; 
  }

  CodeBlob iAdd(ValueIndex first, ValueIndex second) { 
    return CodeBlob{FN_OP_ADD, first, second}; 
  }

  CodeBlob iSubtract(ValueIndex first, ValueIndex second) { 
    return CodeBlob{FN_OP_SUBTRACT, first, second}; 
  }

  CodeBlob iLoad(ValueIndex index) {
    return CodeBlob{FN_OP_LOAD, index};
  }

  CodeBlob iDefHeader(InstructionIndex length) {
    return CodeBlob{FN_OP_DEF, length};
  }

  CodeBlob iCall(ValueIndex index) {
    return CodeBlob{FN_OP_CALL, index};
  }

  CodeBlob iReturnLast() {
    return CodeBlob{FN_OP_RETURN_LAST};
  }

}}
