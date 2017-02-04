#pragma once

#include "stdlib.h"
#include <stack>

// The smallest unit of instruction.
// (Note that instructions are at least this size,
// but are usually more.)
typedef char fnByte;

// Opcodes
typedef fnByte fnOp;
#define FN_OP_FALSE (fnOp)(0)
#define FN_OP_TRUE (fnOp)(1)
#define FN_OP_NUMBER (fnOp)(2)
#define FN_OP_STRING (fnOp)(3)

// The Number struct
typedef signed char fnExponent;
typedef signed long long int fnCoefficient;

class fnVMNumber {
public:
  fnExponent exponent;
  fnCoefficient coefficient;

  fnVMNumber(fnExponent exponent, fnCoefficient coefficient) {
    this->exponent = exponent;
    this->coefficient = coefficient;
  }

  fnVMNumber() = default;
};

// Values in the fn VM.
typedef union _fnVMValue {
  bool asBool;
  fnVMNumber asNumber;
} fnVMValue;

class fnVM {
public:

  fnVM(bool debug);
  fnVM() : fnVM(false) {};
  ~fnVM();

  // Executes a number of instructions.
  // Returns the pointer to the return value.
  fnVMValue run(fnByte instructions[], size_t num_bytes);

protected:
  bool debug;
  std::stack<fnVMValue*> values;

  fnVMValue* declareBool(fnByte value);
  fnVMValue* declareNumber(fnByte value[]);
};
