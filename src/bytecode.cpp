#include "src/bytecode.h"

#include <cstdarg>

namespace fn { namespace bytecode {

  NameHash hashName(std::string name) { 
    return (NameHash)std::hash<std::string>{}(name);
  }

  CodeBlob iFalse() { return CodeBlob{FN_OP_FALSE}; }
  CodeBlob iTrue() { return CodeBlob{FN_OP_TRUE}; }
  CodeBlob iAnd() { return CodeBlob{FN_OP_AND}; }
  CodeBlob iOr() { return CodeBlob{FN_OP_OR}; }
  CodeBlob iNot() { return CodeBlob{FN_OP_NOT}; }

  CodeBlob iNumber(Number number) {
    return iNumber(number.coefficient, number.exponent);
  }

  CodeBlob iNumber(Coefficient coefficient, Exponent exponent) {
    std::array<CodeByte, 10> bytes = std::array<CodeByte, 10>();
    bytes[0] = FN_OP_NUMBER;
    bytes[1] = exponent;
    bytes[2] = coefficient;

    return CodeBlob(bytes);
  }

  CodeBlob iMultiply() { return CodeBlob{FN_OP_MULTIPLY}; }
  CodeBlob iDivide() { return CodeBlob{FN_OP_DIVIDE}; }
  CodeBlob iAdd() { return CodeBlob{FN_OP_ADD}; }
  CodeBlob iSubtract() { return CodeBlob{FN_OP_SUBTRACT}; }
  CodeBlob iEq() { return CodeBlob{FN_OP_EQ}; }

  CodeBlob iName(std::string name) { return iName(hashName(name)); }
  CodeBlob iName(NameHash name) { return CodeBlob{FN_OP_NAME, name}; }
  CodeBlob iLoad(std::string name) { return iLoad(hashName(name)); }
  CodeBlob iLoad(NameHash name) { return CodeBlob{FN_OP_LOAD, name}; }

  CodeBlob iDefHeader(InstructionIndex length, std::vector<std::string> argNames) {
    
    std::vector<NameHash> argNameHashes = std::vector<NameHash>();
    for(auto argName : argNames) {
      argNameHashes.push_back(hashName(argName));
    }

    return iDefHeader(length, argNameHashes);
  }

  CodeBlob iDefHeader(InstructionIndex length, std::vector<NameHash> argNames) {
    CodeBlob blob = CodeBlob{FN_OP_DEF};
    blob.append(length);
    
    NumArgs numArgs = (NumArgs)argNames.size();
    blob.append(numArgs);

    for(auto argName : argNames) {
      blob.append(argName);
    }

    return blob;
  }
  
  CodeBlob iCall() { return CodeBlob{FN_OP_CALL}; }
  CodeBlob iReturnLast() { return CodeBlob{FN_OP_RETURN_LAST}; }

  CodeBlob iJumpIfLastFalse(InstructionIndex jump) { return CodeBlob{FN_OP_FALSE_JUMP, jump}; }

}}
